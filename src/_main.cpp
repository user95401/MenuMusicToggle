#include "mod_utils.hpp"
#include "hooks.hpp"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

class OptionsLayerExt : public OptionsLayer {
public:
    void onMenuMusicToggle(cocos2d::CCObject* pSender) {
        if (GameManager::sharedState()->getGameVariable("0122")) {
            GameManager::sharedState()->setGameVariable("0122", false);
            GameSoundManager::sharedState()->playBackgroundMusic(true, "menuLoop.mp3");
        }
        else {
            GameManager::sharedState()->setGameVariable("0122", true);
            GameSoundManager::sharedState()->stopBackgroundMusic();
        }
    }
};

void(__thiscall* OptionsLayer_customSetup)(OptionsLayer*);
void __fastcall  OptionsLayer_customSetup_H(OptionsLayerExt* self) {
    twoTimesLayerCustomSetupEscape(self);
    OptionsLayer_customSetup(self);

    CCMenu* CCMenu_ = CCMenu::create();
    CCMenu_->setPosition({
        (CCDirector::sharedDirector()->getWinSize().width / 2) + 140, 
        (CCDirector::sharedDirector()->getWinSize().height / 2) - 75 
        });
    CCMenu_->setAnchorPoint({ 0,0 });
    CCMenu_->setScale({ 0.7f });
    self->m_pLayer->addChild(CCMenu_);

    auto menuMusicToggle = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCMenu_, menu_selector(OptionsLayerExt::onMenuMusicToggle)
    );
    menuMusicToggle->toggle(bool(GameManager::sharedState()->getGameVariable("0122")));
    CCMenu_->addChild(menuMusicToggle);
    auto label = CCLabelBMFont::create("Menu\nmusic", "bigFont.fnt", 90, kCCTextAlignmentLeft);
    label->setPosition({ 0, 38 });
    label->setScale({ 0.55f });
    CCMenu_->addChild(label);

}

inline void(__thiscall* GameManager_fadeInMusic)(GameManager*, const char*);
void __fastcall GameManager_fadeInMusic_H(GameManager* self, void*, const char* filename) {
    if (bool(GameManager::sharedState()->getGameVariable("0122"))) {
        if (filename == std::string("menuLoop.mp3")) {
            GameSoundManager::sharedState()->stopBackgroundMusic();
            return GameManager_fadeInMusic(self, "");
        }
        //what else here?
    }
    return GameManager_fadeInMusic(self, filename);
}

DWORD WINAPI PROCESS_ATTACH(void* hModule) {
    MH_SafeInitialize();
    HOOK(base + 0x1dd420, OptionsLayer_customSetup);
    HOOK(base + 0xC4BD0, GameManager_fadeInMusic);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, PROCESS_ATTACH, hModule, 0, 0);
    return TRUE;
}
