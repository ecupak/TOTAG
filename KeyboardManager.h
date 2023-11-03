#pragma once


struct KeyPacket
{
	KeyPacket(const int boundKeycode)
		: boundKeycode_{ boundKeycode }
	{	}

	
	int boundKeycode_{ -1 };


	void Reset()
	{
		wasPressed = false;
		wasReleased = false;
	}


	void BigReset()
	{
		wasPressed = false;
		wasReleased = false;
		
		isPressed = false;
		isJustPressed = false;
		isJustReleased = false;
	}


	bool wasPressed{ false };
	bool wasReleased{ false };

	bool isPressed{ false };
	
	bool isJustPressed{ false };
	bool isJustReleased{ false };
};


// These actions are tied to the keyPackets of corresponding index.
// keyPackets_[0] will be the Jump action, etc.
enum Action
{
	Jump = 0,
	Ability,
	Throw,
	Left,
	Right,
	Up,
	Down,
	ZoomIn,
	ZoomOut,
	Rewind,
	Save,
	Load,
	Count,
	None,
};


class KeyboardManager
{
public:
	KeyboardManager();

	void Update();

	void KeyPressed(const int keycode);

	void KeyReleased(const int keycode);

	const bool IsPressed(const Action action) const;

	const bool IsJustPressed(const Action action) const;

	const bool IsJustReleased(const Action action) const;

	void SetKeyBinding(const Action action, const int newKeycode);

	void ReleaseAll();


private:
	const int GetPacketIndex(const int keycode) const;

	// Initial keybinding should match the action of the keyPacket.
	// keyPackets_[0] is jump and should have a corresponding keybinding.
	KeyPacket keyPackets_[Action::Count]
	{
		GLFW_KEY_J,
		GLFW_KEY_K,
		GLFW_KEY_L,
		
		GLFW_KEY_A,
		GLFW_KEY_D,
		GLFW_KEY_W,
		GLFW_KEY_S,

		GLFW_KEY_MINUS,
		GLFW_KEY_EQUAL,

		GLFW_KEY_I,

		GLFW_KEY_C,
		GLFW_KEY_V,
	};
};