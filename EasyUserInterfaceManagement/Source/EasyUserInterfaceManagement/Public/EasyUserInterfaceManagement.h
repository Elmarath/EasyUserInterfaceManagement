// Copyright Elmarath Studio 2025 All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

// Declare log category for the Easy User Interface Management module
DECLARE_LOG_CATEGORY_EXTERN(LogEasyUserInterfaceManagement, Log, All);

class FEasyUserInterfaceManagementModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
