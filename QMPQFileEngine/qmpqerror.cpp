#include "qmpqerror.h"

#include <StormLib/StormLib.h>

/*QMPQError::QMPQError()
{
}*/

const char * QMPQError::ErrString(int err)
{
    switch (err)
    {
                case ERROR_INVALID_FUNCTION:
        return "function not implemented";
                case ERROR_FILE_NOT_FOUND:
        return "file not found";
                case ERROR_ACCESS_DENIED:
        return "access denied";
                case ERROR_NOT_ENOUGH_MEMORY:
        return "not enough memory";
                case ERROR_BAD_FORMAT:
        return "bad format";
                case ERROR_NO_MORE_FILES:
        return "no more files";
                case ERROR_HANDLE_EOF:
        return "access beyond EOF";
                case ERROR_HANDLE_DISK_FULL:
        return "no space left on device";
                case ERROR_INVALID_PARAMETER:
        return "invalid parameter";
                case ERROR_DISK_FULL:
        return "no space left on device";
                case ERROR_ALREADY_EXISTS:
        return "file exists";
                case ERROR_CAN_NOT_COMPLETE:
        return "operation cannot be completed";
                case ERROR_INSUFFICIENT_BUFFER:
        return "insufficient buffer";
                case ERROR_WRITE_FAULT:
        return "unable to write to device";
                default:
        return "unknown error";
    }
}
