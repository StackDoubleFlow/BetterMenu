#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "System/ValueType.hpp"
#include "System/Func_1.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/LoadSceneMode.hpp"
#include "UnityEngine/Events/UnityAction_2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "UnityEngine/Vector3.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "HMUI/ButtonSpriteSwap.hpp"
#include "images.h"

// Only used for Base64ToSprite, remove once questui updates
#include "System/Convert.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/TextureFormat.hpp"
#include "UnityEngine/ImageConversion.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector4.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Rect.hpp"

using namespace System;
using namespace System::Collections;
using namespace UnityEngine::SceneManagement;
using namespace UnityEngine::Events;
using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace HMUI;

static ModInfo modInfo;

const Logger &getLogger() {
    static const Logger &logger(modInfo);
    return logger;
}

// This is temporary and should be replaced with questui once updated
Sprite* Base64ToSprite(std::string base64, int width, int height) {
    ::Array<uint8_t>* bytes = System::Convert::FromBase64String(il2cpp_utils::createcsstr(base64));
    Texture2D* texture = Texture2D::New_ctor(width, height, TextureFormat::RGBA32, false, false);
    if(ImageConversion::LoadImage(texture, bytes, false))
        return Sprite::Create(texture, UnityEngine::Rect(0.0f, 0.0f, (float)width, (float)height), UnityEngine::Vector2(0.5f,0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, UnityEngine::Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
    return nullptr;
}

DECLARE_CLASS_INTERFACES(Il2CppNamespace, OrganizeEnumerator, "System", "Object", sizeof(Il2CppObject),
    il2cpp_utils::GetClassFromName("System.Collections", "IEnumerator"),

    DECLARE_INSTANCE_FIELD(Il2CppObject*, current);
    DECLARE_INSTANCE_FIELD(bool, hasWaited);

    DECLARE_OVERRIDE_METHOD(bool, MoveNext, il2cpp_utils::FindMethod("System.Collections", "IEnumerator", "MoveNext"));
    DECLARE_OVERRIDE_METHOD(Il2CppObject*, get_Current, il2cpp_utils::FindMethod("System.Collections", "IEnumerator", "get_Current"));
    DECLARE_OVERRIDE_METHOD(void, Reset, il2cpp_utils::FindMethod("System.Collections", "IEnumerator", "Reset"));

    REGISTER_FUNCTION(OrganizeEnumerator,
        getLogger().debug("Registering OrganizeEnumerator!");

        REGISTER_FIELD(current);
        REGISTER_FIELD(hasWaited);

        REGISTER_METHOD(MoveNext);
        REGISTER_METHOD(get_Current);
        REGISTER_METHOD(Reset);
    )
)

DEFINE_CLASS(Il2CppNamespace::OrganizeEnumerator);

Il2CppObject* Il2CppNamespace::OrganizeEnumerator::get_Current() {
    return current;
}

void Il2CppNamespace::OrganizeEnumerator::Reset() {}

bool Il2CppNamespace::OrganizeEnumerator::MoveNext() {
    if (!hasWaited) {
        // TODO: Use MakeFunc once fixed
        auto go_find = il2cpp_utils::MakeDelegate<Func_1<bool>*>(classof(Func_1<bool>*), (void*)nullptr, +[] {
            return GameObject::Find(il2cpp_utils::createcsstr("MainButtons")) != nullptr;
        });
        current = WaitUntil::New_ctor(go_find);
        hasWaited = true;
        return true; // Continue coroutine
    }

    GameObject *main_buttons = GameObject::Find(il2cpp_utils::createcsstr("MainButtons"));

    Transform *campaign = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("CampaignButton"));
    Transform *solo = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("SoloButton"));
    Transform *online = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("OnlineButton"));
    Transform *party = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("PartyButton"));
    Transform *options = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("OptionsButton"));
    Transform *help = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("HelpButton"));
    Transform *exit = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("ExitButton"));

    // Set button positions
    campaign->set_position(UnityEngine::Vector3(-0.85f, 1.07f, 2.6f));
    solo->set_position(UnityEngine::Vector3(-0.22f, 1.07f, 2.60f));
    online->set_position(UnityEngine::Vector3(0.72f, 1.6f, 2.60f));
    party->set_position(UnityEngine::Vector3(0.62f, 1.05f, 2.60f));
    options->set_position(UnityEngine::Vector3(-0.66f, 0.75f, 2.60f));
    help->set_position(UnityEngine::Vector3(0.255f, 0.63f, 2.60f));
    exit->set_position(UnityEngine::Vector3(0.83f, 0.63f, 2.60f));

    // Rescale Settings and Exit buttons
    options->set_localScale(UnityEngine::Vector3(1.35f, 1.35f, 1.0f));
    help->set_localScale(UnityEngine::Vector3(2.35f, 2.3f, 1.0f));
    exit->set_localScale(UnityEngine::Vector3(2.35f, 2.3f, 1.0f));

    // Change solo sprites
    ButtonSpriteSwap *solo_sprite_swap = solo->GetComponentInChildren<ButtonSpriteSwap*>();
    Sprite *solo_normal_sprite = Base64ToSprite(SoloNormal, 288, 508);
    Sprite *solo_active_sprite = Base64ToSprite(SoloActive, 288, 508);
    solo_sprite_swap->normalStateSprite = solo_normal_sprite;
    solo_sprite_swap->highlightStateSprite = solo_active_sprite;
    solo_sprite_swap->images->values[0]->set_sprite(solo_normal_sprite);

    // Change options sprites
    ButtonSpriteSwap *settings_sprite_swap = options->GetComponentInChildren<ButtonSpriteSwap*>();
    Sprite *settings_normal_sprite = Base64ToSprite(SettingsNormal, 584, 113);
    Sprite *settings_active_sprite = Base64ToSprite(SettingsActive, 584, 113);
    settings_sprite_swap->normalStateSprite = settings_normal_sprite;
    settings_sprite_swap->highlightStateSprite = settings_active_sprite;
    settings_sprite_swap->images->values[0]->set_sprite(settings_normal_sprite);

    // Change exit sprites
    ButtonSpriteSwap *exit_sprite_swap = exit->GetComponentInChildren<ButtonSpriteSwap*>();;
    Sprite *exit_normal_sprite = Base64ToSprite(ExitNormal, 260, 113);
    Sprite *exit_active_sprite = Base64ToSprite(ExitActive, 260, 113);
    exit_sprite_swap->normalStateSprite = exit_normal_sprite;
    exit_sprite_swap->highlightStateSprite = exit_active_sprite;
    exit_sprite_swap->images->values[0]->set_sprite(exit_normal_sprite);

    // Change help sprites
    ButtonSpriteSwap *help_sprite_swap = help->GetComponentInChildren<ButtonSpriteSwap*>();;
    Sprite *help_normal_sprite = Base64ToSprite(HelpNormal, 260, 113);
    Sprite *help_active_sprite = Base64ToSprite(HelpActive, 260, 113);
    help_sprite_swap->normalStateSprite = help_normal_sprite;
    help_sprite_swap->highlightStateSprite = help_active_sprite;
    help_sprite_swap->images->values[0]->set_sprite(help_normal_sprite);

    current = nullptr;
    return false; // Reached end of coroutine
}

IEnumerator *Organize() {
    Il2CppNamespace::OrganizeEnumerator *coroutine = CRASH_UNLESS(il2cpp_utils::New<Il2CppNamespace::OrganizeEnumerator *>());
    return reinterpret_cast<IEnumerator *>(coroutine);
}

void OnLoad(UnityEngine::SceneManagement::Scene scene, LoadSceneMode mode) {
    if (to_utf8(csstrtostr(scene.get_name())) == "MenuViewControllers") {
        getLogger().info("Main Menu scene loaded, starting coroutine...");
        SharedCoroutineStarter::get_instance()->StartCoroutine(Organize());
    }
}

extern "C" void setup(ModInfo &info) {
    info.id = "BetterMenu";
    info.version = "0.1.0";
    modInfo = info;
}

extern "C" void load() {
    getLogger().info("Installing BetterMenu Hooks!");

    il2cpp_functions::Init();

    auto on_load = il2cpp_utils::MakeDelegate<UnityAction_2<UnityEngine::SceneManagement::Scene, LoadSceneMode>*>(
        il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_class<UnityAction_2<UnityEngine::SceneManagement::Scene, LoadSceneMode>*>::get(),
        (void*)nullptr, OnLoad);
    SceneManager::add_sceneLoaded(on_load);

    CRASH_UNLESS(custom_types::Register::RegisterType<Il2CppNamespace::OrganizeEnumerator>());

    getLogger().info("Installed BetterMenu Hooks!");
}