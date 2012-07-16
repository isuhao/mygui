/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#include "Precompiled.h"
#include "SettingsResourcePathsControl.h"
#include "SettingsManager.h"
#include "Localise.h"
#include "FactoryManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SettingsResourcePathsControl)

	SettingsResourcePathsControl::SettingsResourcePathsControl() :
		mResourcePathAdd(nullptr),
		mResourcePathDelete(nullptr),
		mResourcePaths(nullptr),
		mOpenSaveFileDialog(nullptr)
	{
	}

	SettingsResourcePathsControl::~SettingsResourcePathsControl()
	{
		mResourcePathAdd->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsResourcePathsControl::notifyClickResourcePathAdd);
		mResourcePathDelete->eventMouseButtonClick -= MyGUI::newDelegate(this, &SettingsResourcePathsControl::notifyClickResourcePathDelete);

		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;
	}

	void SettingsResourcePathsControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mResourcePathAdd, "ResourcePathAdd");
		assignWidget(mResourcePathDelete, "ResourcePathDelete");
		assignWidget(mResourcePaths, "ResourcePaths");

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->Initialise("OpenSaveFileDialog2.layout");
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionOpenFolder"), replaceTags("ButtonOpenFolder"), true);
		mOpenSaveFileDialog->eventEndDialog = MyGUI::newDelegate(this, &SettingsResourcePathsControl::notifyEndDialogOpenSaveFile);

		mResourcePathAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsResourcePathsControl::notifyClickResourcePathAdd);
		mResourcePathDelete->eventMouseButtonClick += MyGUI::newDelegate(this, &SettingsResourcePathsControl::notifyClickResourcePathDelete);
	}

	void SettingsResourcePathsControl::loadSettings()
	{
		mResourcePaths->removeAllItems();
		SettingsSector::VectorUString paths = SettingsManager::getInstance().getSector("Settings")->getPropertyValueList("AdditionalPaths");
		for (SettingsSector::VectorUString::const_iterator item = paths.begin(); item != paths.end(); ++ item)
			mResourcePaths->addItem(*item);
	}

	void SettingsResourcePathsControl::saveSettings()
	{
		SettingsSector::VectorUString paths;
		for (size_t index = 0; index < mResourcePaths->getItemCount(); ++ index)
			paths.push_back(mResourcePaths->getItemNameAt(index));
		SettingsManager::getInstance().getSector("Settings")->setPropertyValueList("AdditionalPaths", paths);
	}

	void SettingsResourcePathsControl::notifyClickResourcePathAdd(MyGUI::Widget* _sender)
	{
		mOpenSaveFileDialog->doModal();
	}

	void SettingsResourcePathsControl::notifyClickResourcePathDelete(MyGUI::Widget* _sender)
	{
		size_t index = mResourcePaths->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
			mResourcePaths->removeItemAt(index);
	}

	void SettingsResourcePathsControl::notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result)
	{
		if (_result)
			mResourcePaths->addItem(mOpenSaveFileDialog->getCurrentFolder());

		mOpenSaveFileDialog->endModal();
	}

	void SettingsResourcePathsControl::OnCommand(const std::string& _command)
	{
		Control::OnCommand(_command);

		if (_command == "Command_LoadSettings")
			loadSettings();
		else if (_command == "Command_SaveSettings")
			saveSettings();
	}

}
