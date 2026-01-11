#pragma once

namespace MRI::FactoryUtility
{
	void RegisterAll();

	void RegisterComponentFactory           ();
	void RegisterStrategyFactory            ();
	void RegisterInterpolatorModifierFactory();
	void RegisterObserverNotifierFactory    ();
	void RegisterComponentModeFactory       ();
}