#include <functional>
#include "Game.h"
#include "Application.h"
#include "Menu.h"
#include "UIComponents.h"
#include "SettingsPage.h"

namespace ApplesGame
{
	SettingsPage::SettingsPage()
	{
		header = new PageHeader("Настройки");

		auto handler = [](SettingsFlags id, bool value) {
			switch (id) {
			case SettingsFlags::isMusicOn:
			{
				State::Instance()->soundManager->SetBackgroundMusicVolume(value ? 50.f : 0.f);
				break;
			}
			case SettingsFlags::isSoundOn:
			{
				State::Instance()->soundManager->SetSoundsVolume(value ? 50.f : 0.f);
				break;
			}
			default:
				break;
			}
			};

		menu = new CheckboxMenu<SettingsFlags>(handler);

		auto state = State::Instance();

		menu->AddItem(SettingsFlags::isMusicOn, "Музыка", state->setings.getIsMusicOn());
		menu->AddItem(SettingsFlags::isSoundOn, "Звуки", state->setings.getisSoundOn());

		menu->Hover(0);

		backText = new CommonText("<TAB> Назад", { 0.5f, 0.f });
	}

	void SettingsPage::Init()
	{
	}

	void SettingsPage::HandleKeyboardEvent(const sf::Event& evt)
	{
		if (evt.type == sf::Event::KeyReleased && evt.key.code == sf::Keyboard::Tab)
		{
			State::Instance()->popGameState();
		}

		menu->HandleKeyboardEvent(evt);
	}

	void SettingsPage::Draw(Vector2f margin)
	{
		float windowX = (float)Application::Instance()->GetWindow().getSize().x;
		float windowY = (float)Application::Instance()->GetWindow().getSize().y;

		Vector2f menuPosition = { windowX / 2, windowY / 3 };
		Vector2f headerPosition = { menuPosition.x, menuPosition.y - 50.f };

		menu->Draw(menuPosition);
		header->Draw(headerPosition);
		backText->Draw({ windowX / 2, windowY - 100.f });
	}
}