/* Copyright 2007-2015 QReal Research Group
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

#include "generatorForListsOfElements.h"

#include "generatorForProperties.h"
#include "generatorForEdgeElements.h"
#include "generatorForNodeElements.h"
#include "generatorForExpressions.h"

using namespace qReal;
using namespace constraints::generator::generatorParts;

QPair<QString, QStringList > GeneratorForListsOfElements::countConstraintForChildrens(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "Children"
			, "children"
			, "node"
			, depth
			, additionalString
			, api
			, errorReporter
			, countsOfConstraintElementsInOneConstraint);
}

QPair<QString, QStringList > GeneratorForListsOfElements::countConstraintForOutgoingLinks(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "OutgoingLinks"
			, "outgoingLinks"
			, "edge"
			, depth
			, additionalString
			, api
			, errorReporter
			, countsOfConstraintElementsInOneConstraint);
}

QPair<QString, QStringList > GeneratorForListsOfElements::countConstraintForIncomingLinks(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "IncomingLinks"
			, "incomingLinks"
			, "edge"
			, depth
			, additionalString
			, api
			, errorReporter
			, countsOfConstraintElementsInOneConstraint);
}

QPair<QString, QStringList > GeneratorForListsOfElements::countConstraintForOutgoingNodes(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "OutgoingNodes"
			, "outgoingNodes"
			, "node"
			, depth
			, additionalString
			, api
			, errorReporter
			, countsOfConstraintElementsInOneConstraint);
}

QPair<QString, QStringList > GeneratorForListsOfElements::countConstraintForIncomingNodes(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "IncomingNodes"
			, "incomingNodes"
			, "node"
			, depth
			, additionalString
			, api
			, errorReporter
			, countsOfConstraintElementsInOneConstraint);
}

QPair<QString, QStringList > GeneratorForListsOfElements::countConstraintForListOfElements(
		const Id &constraint
		, const QString &elementName
		, const QString &resultElementName
		, const QString &functionName
		, const QString &resultType
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint)
{
	QString resultString = "";
	QStringList listOfBooleanExpressions;
	QStringList allResultBool;

	QString count = api.property(constraint, "count").toString();
	bool neededCount  = (!count.isEmpty());

	if (neededCount) {
		if (QRegExp("(([<>=]|&lt;|&rt;)=?)\\s*(\\d+)").exactMatch(count)) {
			count.replace(QRegExp("([<>=&rlt;]+)(\\d+)"), "\\1 \\2");
		} else {
			errorReporter.addCritical(QObject::tr("Value \"%1\" is not correct for property 'Count'.").arg(count)
					, constraint);
			neededCount = false;
		}
	}

	if (neededCount) {
		resultString += QString("%1int tempCount%2_%3 = 0;\n")
				.arg(additionalString)
				.arg(resultElementName)
				.arg(depth);
	}

	if (functionName == "outgoingNodes") {
		resultString += codeForOutgoingOfIncomingNodes(
				"outgoingLinks"
				, elementName
				, additionalString
				, resultElementName
				, depth);
	} else if (functionName == "incomingNodes") {
		resultString += codeForOutgoingOfIncomingNodes(
				"incomingLinks"
				, elementName
				, additionalString
				, resultElementName
				, depth);
	} else {
		resultString += QString("%1qReal::IdList new%2NamesList_%3 = logicalApi.%4(%5);\n")
				.arg(additionalString)
				.arg(resultElementName)
				.arg(depth)
				.arg(functionName)
				.arg(elementName);
	}

	resultString += QString("%1bool main%2Res_%3 = true;\n")
			.arg(additionalString)
			.arg(resultElementName)
			.arg(depth + 1);

	const QString currentListElement = resultType
			+ resultElementName
			+ "_"
			+ QString::number(depth);

	resultString += QString("%1foreach (qReal::Id const &%2, new%3NamesList_%4) {\n")
			.arg(additionalString)
			.arg(currentListElement)
			.arg(resultElementName)
			.arg(depth);

	const QString selection = api.property(constraint, "selection").toString();
	const bool neededSelection  = (!selection.isEmpty()) && (selection.compare("all", Qt::CaseInsensitive) != 0);

	if (neededSelection) {
		QStringList selectionList = selection.split(" ");
		QPair<QString, QString> selectionRes = GeneratorForProperties::countPropertyCharacteristicForConstraintElement(
				constraint
				, resultElementName.at(0).toLower() + resultElementName.mid(1) + "Selection"
				, "false"
				, selectionList.at(0)
				, selectionList.at(1)
				, selectionList.at(2)
				, currentListElement
				, depth
				, additionalString + "	"
				, errorReporter);

		resultString += QString("%1%2	if (%3) {\n")
				.arg(selectionRes.first)
				.arg(additionalString)
				.arg(selectionRes.second);
	}

	if (resultType == "node") {
		resultString += GeneratorForNodeElements::countRealConstraintForNodeElement(
				constraint
				, currentListElement
				, functionName + "Res"
				, depth + 1
				, additionalString + "	"
				, api
				, countsOfConstraintElementsInOneConstraint
				, errorReporter);
	} else if (resultType == "edge") {
		resultString += GeneratorForEdgeElements::countRealConstraintForEdgeElement(
				constraint
				, currentListElement
				, functionName + "Res"
				, depth + 1
				, additionalString + "	"
				, api
				, countsOfConstraintElementsInOneConstraint
				, errorReporter);
	}

	resultString += QString("%1	main%2Res_%3 = main%2Res_%3 && %4Res_%3;\n")
			.arg(additionalString)
			.arg(resultElementName)
			.arg(depth + 1)
			.arg(functionName);

	if (neededCount) {
		resultString += QString("%1	tempCount%2_%3++;\n")
				.arg(resultElementName)
				.arg(additionalString)
				.arg(depth);
	}

	if (neededSelection) {
		resultString += QString("%1	}\n").arg(additionalString);
	}

	resultString += additionalString + "}\n";
	listOfBooleanExpressions.push_back("main" + resultElementName + "Res_" + QString::number(depth + 1));

	if (neededCount) {
		QStringList countList = count.split(" ");
		QString sign = countList.at(0);

		if (sign == "=") {
			sign += "=";
		}

		sign.replace("&lt;", "<");

		resultString += QString("%1bool count%2Res_%3 = (tempCount%2_%3 %4 %5);\n")
				.arg(additionalString)
				.arg(resultElementName)
				.arg(depth)
				.arg(sign)
				.arg(countList.at(1));
		listOfBooleanExpressions.push_back("count" + resultElementName + "Res_" + QString::number(depth));
	}

	allResultBool.append(GeneratorForExpressions::conjunctionExpression(listOfBooleanExpressions));

	return {resultString, allResultBool};
}

QString GeneratorForListsOfElements::codeForOutgoingOfIncomingNodes(
		const QString &listOfLinks
		, const QString &elementName
		, const QString &additionalString
		, const QString &resultElementName
		, const int depth)
{
	QString resultString;

	const QString helperListName = QString("helperListOfLinks_%1").arg(depth);

	resultString += QString("%1qReal::IdList %2 = logicalApi.%3(%4);\n")
			.arg(additionalString)
			.arg(helperListName)
			.arg(listOfLinks)
			.arg(elementName);

	const QString listName = QString("new%1NamesList_%2")
			.arg(resultElementName)
			.arg(depth);

	resultString += QString("%1qReal::IdList %2;\n")
			.arg(additionalString)
			.arg(listName);

	resultString += QString("%1foreach (qReal::Id const &linkId, %2) {\n")
			.arg(additionalString)
			.arg(helperListName);

	const QString property = (listOfLinks == "outgoingLinks") ? "to" : "from";
	resultString += QString("%1	qReal::Id newNodeInList = logicalApi.%2(linkId);\n")
			.arg(additionalString)
			.arg(property);

	resultString += QString("%1	%2.append(newNodeInList);\n")
			.arg(additionalString)
			.arg(listName);

	resultString += QString("%1}\n")
			.arg(additionalString);

	return resultString;
}
