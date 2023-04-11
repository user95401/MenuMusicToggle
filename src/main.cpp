#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <random>
#include <cocos2d.h>
#include <gd.h>
#include "mod_utils.hpp"
#include "mapped-hooks.hpp"
#include <MinHook.h>
using namespace cocos2d;
using namespace gd;
using namespace cocos2d::extension;
class GJOptionsLayerHook {
public:
    void onMenuMusicToggle(cocos2d::CCObject* pSender) {
        if (GameManager::sharedState()->getGameVariable("0122")) {
            GameManager::sharedState()->setGameVariable("0122", false);
            GameManager::sharedState()->fadeInMusic("menuLoop.mp3");
        }
        else {
            GameManager::sharedState()->setGameVariable("0122", true);
            GameSoundManager::sharedState()->stopBackgroundMusic();
        }
    }
};
void __fastcall  OptionsLayer_customSetup(GJDropDownLayer* self) {
    MHook::getOriginal(OptionsLayer_customSetup)(self);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    auto m_pLayer = self->m_pLayer;
    auto m_pListLayer = self->m_pListLayer;

    CCMenu* m = CCMenu::create();
    m->setPosition({ (winSize.width / 2) + 140, (winSize.height / 2) - 75 });
    m->setAnchorPoint({ 0,0 });
    m->setScale({ 0.7f });
    m_pLayer->addChild(m);

    auto menuMusicToggle = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        m,
        (cocos2d::SEL_MenuHandler)&GJOptionsLayerHook::onMenuMusicToggle
    );
    menuMusicToggle->toggle(bool(GameManager::sharedState()->getGameVariable("0122")));
    m->addChild(menuMusicToggle);
    auto label = CCLabelBMFont::create("Menu\nmusic", "bigFont.fnt", 90, kCCTextAlignmentLeft);
    label->setPosition({ 0, 38 });
    label->setScale({ 0.55f });
    m->addChild(label);
}
DWORD WINAPI thread_func(void* hModule) {
    // initialize minhook
    MH_Initialize();

    std::random_device os_seed;
    const unsigned int seed = os_seed();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribute(250, 1000);
    int sleepMs = distribute(generator);
    Sleep(sleepMs);

    MHook::registerHook(base + 0x1dd420, OptionsLayer_customSetup);

    // enable all hooks you've created with minhook
    MH_EnableHook(MH_ALL_HOOKS);

    while (true)
    {
        if (FMODAudioEngine::sharedEngine()->isBackgroundMusicPlaying(std::string("menuLoop.mp3"))) { //????
            if (GameManager::sharedState()->getGameVariable("0122")) {
                GameSoundManager::sharedState()->stopBackgroundMusic();
            }
        }
    }

    return 0;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, thread_func, hModule, 0, 0);
    return TRUE;
}
