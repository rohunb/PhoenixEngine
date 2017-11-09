#ifndef PHOENIX_WIN32_WINDOW_H
#define PHOENIX_WIN32_WINDOW_H

#include "Utility/Misc/Function.h"
#include "Utility/Misc/Memory.h"
#include "Platform/Window/IWindow.h"

typedef struct GLFWwindow FGLWindow;

namespace Phoenix
{
	class FGenericWindow : public IWindow
	{
	public:

		FGenericWindow() = delete;
		FGenericWindow(int Width, int Height, const FString& WindowTitle);

		virtual ~FGenericWindow();
		
		virtual void Hide() final;
		virtual void Show() final;

		virtual void Minimize() final;
		virtual void Restore() final;

		virtual void OnMinimize() final;
		virtual void OnRestore() final;

		virtual void SetFullScreen(const bool bFullScreenEnabled) final;
		
		virtual void ProcessEvents() final;

		virtual void PollMovementAxes() final;

		virtual void SetDimensions(const FVector2D& InDimensions) final;
		virtual const FVector2D& GetDimensions() const final;

		virtual void SetPosition(const FVector2D& InPosition) final;
		virtual const FVector2D& GetPosition() const final;

		virtual void SetWindowTitle(const FString& InWindowTitleText) final;
		virtual const FString& GetWindowTitle() const final;

		virtual void SetTaskbarIcon(const FIcon& InIcon) final;
		virtual const FIcon& GetTaskbarIcon() const final;

		virtual void SetTitlebarIcon(const FIcon& InIcon) final;
		virtual const FIcon& GetTitlebarIcon() const final;

		virtual void SetGraphicsContextCurrent() final;

		virtual void BufferSwap() final;

		virtual void SetSwapInterval(const Int32 Interval) final;

		virtual void SetEventHandler(const TRawPtr<class FEventHandler>& InEventHandler) final;
		
	protected:
		virtual void Init() final;

	private:
		FGLWindow* Window{ nullptr };
		TRawPtr<class FEventHandler> EventHandler;

		static FGenericWindow& GetClass(GLFWwindow* const Instance);

		static void WindowCloseAttemptCallback(GLFWwindow* Instance);
		static void WindowFocusedCallback(GLFWwindow* Instance, Int32 IsFocused);
		static void WindowFrameBufferResizedCallback(GLFWwindow* Instance, Int32 Width, Int32 Height);
		static void WindowIconifiedCallback(GLFWwindow* Instance, Int32 IsIconified);
		static void WindowPositionChangedCallback(GLFWwindow* Instance, Int32 X, Int32 Y);

		static void WindowRefreshedCallback(GLFWwindow* Instance);
		static void WindowSizeChangedCallback(GLFWwindow* Instance, Int32 Width, Int32 Height);
		static void CursorEnterCallback(GLFWwindow* Instance, Int32 DidEnterArea);
		static void CursorPosCallback(GLFWwindow* Instance, Float64 CursorX, Float64 CursorY);
		static void KeyCallback(GLFWwindow* Instance, Int32 Key, Int32 ScanCode, Int32 Action, Int32 Mods);

		static void MouseButtonCallback(GLFWwindow* Instance, Int32 Button, Int32 Action, Int32 Mods);
		static void ScrollCallback(GLFWwindow* Instance, Float64 DeltaX, Float64 DeltaY);
	};
}

#endif
