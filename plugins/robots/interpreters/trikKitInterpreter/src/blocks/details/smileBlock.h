#pragma once

#include "smileBlockBase.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SmileBlock : public SmileBlockBase
{
public:
	explicit SmileBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
};

}
}
}