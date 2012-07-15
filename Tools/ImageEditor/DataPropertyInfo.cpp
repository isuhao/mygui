/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataPropertyInfo.h"

namespace tools
{

	DataTypeProperty::DataTypeProperty()
	{
	}

	DataTypeProperty::~DataTypeProperty()
	{
	}

	void DataTypeProperty::deserialization(pugi::xml_node _node)
	{
		mName = _node.select_single_node("Name").node().child_value();
		mType = _node.select_single_node("Type").node().child_value();
		mDefaultValue = _node.select_single_node("Default").node().child_value();
	}

	const std::string& DataTypeProperty::getName() const
	{
		return mName;
	}

	const std::string& DataTypeProperty::getType() const
	{
		return mType;
	}

	const std::string& DataTypeProperty::getDefaultValue() const
	{
		return mDefaultValue;
	}

}
