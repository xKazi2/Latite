#pragma once
#include "../Screen.h"
#include "util/LMath.h"
#include "util/DxUtil.h"
#include "client/feature/module/HudModule.h"

class HUDEditor : public Screen {
public:
	HUDEditor();
	void onEnable(bool ignoreAnims) override;
	void onDisable() override;
private:
	void onRender(Event& ev);
	void renderModule(class HUDModule* mod);
	void doDragging();
	void doSnapping(Vec2 const&);
	void keepModulesInBounds();

	class HUDModule* dragMod;

	float anim = 0.f;
	Vec2 dragOffset = {};

	struct SnapLine {
		float left, middle, right;
		d2d::Color color = d2d::Colors::WHITE;
		float thickness = 1.f;

		SnapLine(HUDModule* dragMod, float num, bool horizontal) {
			if (horizontal) {
				float height = dragMod->getRect().getHeight();
				right = num;
				left = num - height;
				middle = num - (height / 2.f);
				return;
			}
			float width = dragMod->getRect().getWidth();
			right = num;
			left = num - width;
			middle = num - (width / 2.f);
		}
	};
};