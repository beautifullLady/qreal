/* Copyright 2015 CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <generatorBase/templateParametrizedEntity.h>

namespace ev3 {
namespace rbf {
namespace lua {

/// Converts the given Lua reserved function call to the target language one.
class Ev3ReservedFunctionsConverter : public generatorBase::TemplateParametrizedEntity
{
public:
	explicit Ev3ReservedFunctionsConverter(const QStringList &pathsToTemplates);

	QString convert(const QString &name, const QStringList &args) const;
};

}
}
}
