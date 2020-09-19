#include <interpreter/error.h>

const char * VerifyError::what() const noexcept {
    return issue.c_str();
}
