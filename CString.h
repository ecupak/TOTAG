#pragma once
class CString
{
public:
    CString(const char* string)
    {
        SetAsString(string);
    }


    CString(char* string)
    {
        SetAsString(string);
    }


    CString(size_t length)
    {
        ResizeString(length);
    }


    ~CString()
    {
        delete[] string_;
    }


    void operator=(CString& other)
    {
        SetAsString(other.string_);
    }


    char* Ptr() const
    {
        return string_;
    }


    // [Credit] https://stackoverflow.com/questions/49801418/default-return-of-class-instance-without-member-function
    operator char* ()
    {
        return string_;
    }


private:
    void SetAsString(const char* string)
    {
        // NOTE - strlen() does not count the null terminator.
        size_t size{ strlen(string) };

        ResizeString(size);

        // Tell copy function the size of destination (string_). It is strlen() + 1: the null terminator.
        strcpy_s(string_, size + 1, string);
    }


    void ResizeString(size_t size)
    {
        // New memeory must request 1 extra for the null terminator.
        delete[] string_;
        string_ = new char[size + 1];
    }

    char* string_{ nullptr };
};

