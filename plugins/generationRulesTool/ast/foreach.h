#pragma once

#include "node.h"

namespace simpleParser {
namespace ast {

/// Class that represents foreach node.
class Foreach : public Node
{
public:
	/// Constructor.
	/// @param identifier - iterator identifier.
	/// @param list - list to iterate.
	/// @param program - subprogram.
	Foreach(QSharedPointer<Node> const &identifier
			, QSharedPointer<Node> const &list
			, QSharedPointer<Node> const &program)
		: mIdentifier(identifier)
		, mList(list)
		, mProgram(program)
	{
	}

	/// Returns identifier node.
	QSharedPointer<Node> identifier() const
	{
		return mIdentifier;
	}

	QSharedPointer<Node> listPart() const
	{
		return mList;
	}

	/// Returns program node.
	QSharedPointer<Node> program() const
	{
		return mProgram;
	}

	QList<QSharedPointer<Node>> children() const override
	{
		return {mIdentifier, mList, mProgram};
	}

private:
	QSharedPointer<Node> mIdentifier;
	QSharedPointer<Node> mList;
	QSharedPointer<Node> mProgram;
};
}
}
