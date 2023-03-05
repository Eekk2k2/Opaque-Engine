#pragma once

/// <summary>
/// The catalyst is like the monobehaviour class in Uniy. Yes the name is AI generated.
/// </summary>
class Catalyst
{
public:

	/// <summary>
	/// Constructor for the catalyst class.
	/// </summary>
	Catalyst();

	/// <summary>
	/// Destructor(?) for the catalyst class.
	/// </summary>
	~Catalyst();

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

	/// <summary>
	/// If isEnabled is true logic is run, if not no logic is run.
	/// </summary>
	bool isEnabled = true;

	/// <summary>
	/// We do not want to enable/disable the script if the object this script is attached is enabled or not because some wierd things will happen
	/// </summary>
	bool isObjectEnabled = true; // TODO : Implement, for now assumes 
};
