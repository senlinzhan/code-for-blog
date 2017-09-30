#ifndef ERROR_H
#define ERROR_H

#include <string> 

class Error
{
public:
    Error()
        : hasError_(false)
    {        
    }

    Error(const std::string &reason)
        : hasError_(true),
          reason_(reason)
    {        
    }

    Error(std::string &&reason) noexcept
        : hasError_(true),
          reason_(std::move(reason))
    {        
    }
 
    operator bool() const
    {
        return hasError_;
    }

    const std::string &reason() const
    {
        return reason_;
    }
    
private:
    bool hasError_;
    std::string reason_;
};

#endif /* ERROR_H */
