#include "pch.h"
#include "MotionBlur.h"
#include "client/render/Renderer.h"
#include "client/Latite.h"

#include "client/event/impl/RendererInitEvent.h"

MotionBlur::MotionBlur() : Module("MotionBlur", "Motion Blur", "Blurs motion", GAME) {
	addSliderSetting("intensity", "Intensity", "", intensity, FloatValue(0.f), FloatValue(10.f), FloatValue(1.f));
	addSliderSetting("bleed", "Anti Bleed", "", antiBleed, FloatValue(0.f), FloatValue(1.f), FloatValue(0.1f));
	
	listen<RendererCleanupEvent>(&MotionBlur::onCleanup);
	listen<RenderOverlayEvent>(&MotionBlur::onRender, true, 10);
	listen<RendererInitEvent>(&MotionBlur::onRendererInit, true);

}

void MotionBlur::onEnable() {
}

void MotionBlur::onDisable() {
}

void MotionBlur::onRender(Event& genericEv) {
	auto& ev = reinterpret_cast<RenderOverlayEvent&>(genericEv);
	auto ctx = ev.getDeviceContext();


	if (this->isEnabled()) {
		auto grainOfSalt = Latite::getRenderer().copyCurrentBitmap();
		if (motionBlurList.size() > 0) {
			for (auto& map : motionBlurList) {
				if (map) {
					auto ss = Latite::getRenderer().getScreenSize();
					auto rc = D2D1::RectF(0.f, 0.f, ss.width, ss.height);
					ctx->DrawBitmap(map, &rc, (std::get<FloatValue>(intensity) / 10.f) - 0.05f);
					ctx->DrawBitmap(grainOfSalt, &rc, std::get<FloatValue>(antiBleed));
				}
			}
			SafeRelease(&motionBlurList.front());
			motionBlurList.erase(motionBlurList.begin());
		}
		motionBlurList.push_back(Latite::getRenderer().copyCurrentBitmap());
		SafeRelease(&grainOfSalt);
	}
	else {
		if (motionBlurList.size() > 0) {
			for (auto& map : motionBlurList) {
				if (map) SafeRelease(&map);
			}
		}
		motionBlurList.clear();
	}
}

void MotionBlur::onCleanup(Event&) {
	//auto& ev = reinterpret_cast<RendererCleanupEvent&>(genericEv);
	//SafeRelease(&mbBitmap);
	//SafeRelease(&oBitmap);

	for (auto& bmp : this->motionBlurList) {
		if (bmp) bmp->Release();
	}

	motionBlurList.clear();
}

void MotionBlur::onRendererInit(Event& genericEv) {
	//this->mbBitmap = Latite::getRenderer().getCopiedBitmap();
	//this->oBitmap = Latite::getRenderer().getCopiedBitmap();
}
