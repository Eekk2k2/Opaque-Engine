#pragma once
#include <string>

class Component
{
public:
	Component();
	~Component();

	/// <summary>
	/// This is what is referenced on the object (i.e. RGun.Components.__ComponentName), I do not know if this is going to be a feature in the future, or if i just remove it and you get the component by referencing the type (i.e. RGun.Components<type>)
	/// </summary>
	std::string ComponentName = "Unnamed Component";

	/// <summary>
	/// If you can remove the component during runtime. This could be useful for the Transform component for instance, because we do not want it to me removed. Defaults to true
	/// </summary>
	bool Removable = true;

	/// <summary>
	/// Can only be one component of this type attached to an object at a time. This can be useful for the Transform component, as the game would choose one either way. This funciton wil raraely be true. Defaults to false.
	/// </summary>
	bool Single = false;

	/// <summary>
	/// If isEnabled is true logic is run, if not no logic is run.
	/// </summary>
	bool isEnabled = true;

	/// <summary>
	/// We do not want to enable/disable the script if the object this script is attached is enabled or not because some wierd things will happen
	/// </summary>
	bool isObjectEnabled = true; // TODO : Implement, for now assumes 

	/// <summary>
	/// Default implementation of the Catalyst::Initialize(); function
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// Default implementation of the Catalyst::Start(); function
	/// </summary>
	virtual void Start();

	/// <summary>
	/// Default implementation of the Catalyst::Update(); function
	/// </summary>
	virtual void Update();

	/// <summary>
	/// Default implementation of the Catalyst::Cleanup(); function
	/// </summary>
	virtual void Cleanup();

private:

};

