#include "network/tls_transport.h"

#if defined(OPENADS_WITH_TLS)

#include "openads/error.h"

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/error.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/pk.h>
#include <mbedtls/ssl.h>
#include <mbedtls/x509_crt.h>

#include <cstring>
#include <utility>

namespace openads::network {

namespace {

inline std::string mbed_msg(int err) {
    char buf[160] = {0};
    mbedtls_strerror(err, buf, sizeof(buf));
    return std::string(buf);
}

class MbedTlsTransport : public ITransport {
public:
    MbedTlsTransport() {
        mbedtls_net_init(&net_);
        mbedtls_ssl_init(&ssl_);
        mbedtls_ssl_config_init(&conf_);
        mbedtls_ctr_drbg_init(&drbg_);
        mbedtls_entropy_init(&entropy_);
        mbedtls_x509_crt_init(&ca_);
        mbedtls_x509_crt_init(&srv_cert_);
        mbedtls_pk_init(&srv_key_);
    }
    ~MbedTlsTransport() override { close(); }
    MbedTlsTransport(const MbedTlsTransport&) = delete;
    MbedTlsTransport& operator=(const MbedTlsTransport&) = delete;

    util::Result<std::size_t>
        send(const std::uint8_t* buf, std::size_t n) override {
        int rc = mbedtls_ssl_write(&ssl_, buf, n);
        if (rc < 0) {
            return util::Error{openads::AE_REMOTE_ERROR, rc,
                "mbedtls_ssl_write: " + mbed_msg(rc), ""};
        }
        return static_cast<std::size_t>(rc);
    }

    util::Result<std::size_t>
        recv(std::uint8_t* buf, std::size_t n) override {
        int rc = mbedtls_ssl_read(&ssl_, buf, n);
        if (rc == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY) return std::size_t{0};
        if (rc < 0) {
            return util::Error{openads::AE_REMOTE_ERROR, rc,
                "mbedtls_ssl_read: " + mbed_msg(rc), ""};
        }
        return static_cast<std::size_t>(rc);
    }

    void close() noexcept override {
        if (closed_) return;
        closed_ = true;
        mbedtls_ssl_close_notify(&ssl_);
        mbedtls_x509_crt_free(&ca_);
        mbedtls_x509_crt_free(&srv_cert_);
        mbedtls_pk_free(&srv_key_);
        mbedtls_ssl_free(&ssl_);
        mbedtls_ssl_config_free(&conf_);
        mbedtls_ctr_drbg_free(&drbg_);
        mbedtls_entropy_free(&entropy_);
        mbedtls_net_free(&net_);
    }

    bool valid() const noexcept override { return !closed_; }

    util::Result<void>
        client_handshake(const std::string& host, std::uint16_t port,
                         const TlsConfig& cfg) {
        const std::string portstr = std::to_string(port);
        int rc = mbedtls_ctr_drbg_seed(&drbg_, mbedtls_entropy_func,
                                       &entropy_, nullptr, 0);
        if (rc != 0) return util::Error{openads::AE_REMOTE_ERROR, rc,
            "ctr_drbg_seed: " + mbed_msg(rc), ""};

        rc = mbedtls_net_connect(&net_, host.c_str(), portstr.c_str(),
                                 MBEDTLS_NET_PROTO_TCP);
        if (rc != 0) return util::Error{openads::AE_REMOTE_ERROR, rc,
            "net_connect: " + mbed_msg(rc), host + ":" + portstr};

        rc = mbedtls_ssl_config_defaults(&conf_, MBEDTLS_SSL_IS_CLIENT,
                                          MBEDTLS_SSL_TRANSPORT_STREAM,
                                          MBEDTLS_SSL_PRESET_DEFAULT);
        if (rc != 0) return util::Error{openads::AE_REMOTE_ERROR, rc,
            "ssl_config_defaults: " + mbed_msg(rc), ""};

        if (cfg.insecure_skip_verify) {
            mbedtls_ssl_conf_authmode(&conf_, MBEDTLS_SSL_VERIFY_NONE);
        } else if (cfg.ca_pem.empty()) {
            mbedtls_ssl_conf_authmode(&conf_, MBEDTLS_SSL_VERIFY_REQUIRED);
        } else {
            rc = mbedtls_x509_crt_parse(&ca_,
                reinterpret_cast<const unsigned char*>(cfg.ca_pem.data()),
                cfg.ca_pem.size() + 1);
            if (rc < 0) return util::Error{openads::AE_REMOTE_ERROR, rc,
                "x509_crt_parse(CA): " + mbed_msg(rc), ""};
            mbedtls_ssl_conf_authmode(&conf_, MBEDTLS_SSL_VERIFY_REQUIRED);
            mbedtls_ssl_conf_ca_chain(&conf_, &ca_, nullptr);
        }
        mbedtls_ssl_conf_rng(&conf_, mbedtls_ctr_drbg_random, &drbg_);

        rc = mbedtls_ssl_setup(&ssl_, &conf_);
        if (rc != 0) return util::Error{openads::AE_REMOTE_ERROR, rc,
            "ssl_setup: " + mbed_msg(rc), ""};

        const std::string& sni = cfg.sni_hostname.empty() ? host
                                                          : cfg.sni_hostname;
        rc = mbedtls_ssl_set_hostname(&ssl_, sni.c_str());
        if (rc != 0) return util::Error{openads::AE_REMOTE_ERROR, rc,
            "ssl_set_hostname: " + mbed_msg(rc), ""};

        mbedtls_ssl_set_bio(&ssl_, &net_, mbedtls_net_send,
                            mbedtls_net_recv, nullptr);

        while ((rc = mbedtls_ssl_handshake(&ssl_)) != 0) {
            if (rc != MBEDTLS_ERR_SSL_WANT_READ &&
                rc != MBEDTLS_ERR_SSL_WANT_WRITE) {
                return util::Error{openads::AE_REMOTE_ERROR, rc,
                    "ssl_handshake (client): " + mbed_msg(rc), host};
            }
        }
        return {};
    }

private:
    mbedtls_net_context        net_{};
    mbedtls_ssl_context        ssl_{};
    mbedtls_ssl_config         conf_{};
    mbedtls_ctr_drbg_context   drbg_{};
    mbedtls_entropy_context    entropy_{};
    mbedtls_x509_crt           ca_{};
    mbedtls_x509_crt           srv_cert_{};
    mbedtls_pk_context         srv_key_{};
    bool                       closed_ = false;
};

} // namespace

util::Result<std::unique_ptr<ITransport>>
connect_tls(const std::string& host, std::uint16_t port,
            const TlsConfig& cfg) {
    auto t = std::make_unique<MbedTlsTransport>();
    if (auto r = t->client_handshake(host, port, cfg); !r) {
        return r.error();
    }
    return std::unique_ptr<ITransport>(std::move(t));
}

} // namespace openads::network

#endif // OPENADS_WITH_TLS
