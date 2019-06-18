#include "ViewProvider.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	auto viewProviderFactory = winrt::make<ViewProviderFactory>();
	CoreApplication::Run(viewProviderFactory);
	return 0;
}