#include "AlarmCode.h"

std::string AlarmCodeToString(AlarmCode code)
{
    switch (code)
    {
    case AlarmCode::NONE:
        return "NONE";

    case AlarmCode::WAFER_NOT_DETECTED:
        return "WAFER_NOT_DETECTED";

    case AlarmCode::WAFER_ALREADY:
        return "WAFER_ALREADY";

    case AlarmCode::EQUIPMENT_NOT_READY:
        return "EQUIPMENT_NOT_READY";

    case AlarmCode::RECIPE_NOT_SET:
        return "RECIPE_NOT_SET";

    case AlarmCode::PROCESS_ALREADY_RUNNING:
        return "PROCESS_ALREADY_RUNNING";
    }

    return "UNKNOWN";
}