// Copyright Elmarath Studio 2025 All Rights Reserved.

#include "EasyUserInterfaceManagement.h"

#define LOCTEXT_NAMESPACE "FEasyUserInterfaceManagementModule"

// Initialize Log Category for Easy User Interface Management
DEFINE_LOG_CATEGORY(LogEasyUserInterfaceManagement);

void FEasyUserInterfaceManagementModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FEasyUserInterfaceManagementModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEasyUserInterfaceManagementModule, EasyUserInterfaceManagement)