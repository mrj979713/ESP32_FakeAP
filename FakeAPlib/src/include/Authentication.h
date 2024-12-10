#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "include.h"


class AuthenticationManager 
{
protected:
    uint8_t       _loginAttempts   = 0;
    unsigned long _lastAttemptTime = 0;

    String hashPassword(const String& password) {
        // Advanced: Implement SHA-256 or bcrypt
        return base64::encode(password);
    }

public:
    inline bool validateAdminCredentials(const String& username, const String& password)
    {
        // Check login attempts and lockout
        if (_loginAttempts >= MAX_LOGIN_ATTEMPTS && 
            (millis() - _lastAttemptTime) < LOGIN_LOCKOUT_TIME) {
            return false;
        }

        bool validCredentials = (
            username == ADMIN_USERNAME && 
            hashPassword(password) == hashPassword(ADMIN_PASSWORD)
        );

        if (!validCredentials) {
            _loginAttempts++;
            _lastAttemptTime = millis();
        } else {
            _loginAttempts = 0;
        }

        return validCredentials;
    }

    inline bool validateSocialLogin(const String& platform, const String& token)
    {
        // Placeholder for OAuth validation
        // In a real implementation, validate against respective social network APIs
        return token.length() > 10;
    }
};

#endif