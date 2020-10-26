#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "System/ValueType.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/SceneManagement/LoadSceneMode.hpp"
#include "UnityEngine/Events/UnityAction_2.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"

using namespace System::Collections;
using namespace UnityEngine::SceneManagement;
using namespace UnityEngine::Events;
using namespace UnityEngine;
using namespace GlobalNamespace;

static ModInfo modInfo;

const Logger &getLogger() {
    static const Logger &logger(modInfo);
    return logger;
}

IEnumerator *Organize() {

}

void OnLoad(UnityEngine::SceneManagement::Scene *scene, LoadSceneMode *mode) {
    getLogger().info("sceneLoaded OnLoad was called!");

    if (scene->get_name()->Equals(il2cpp_utils::createcsstr("test"))) {
        SharedCoroutineStarter::get_instance()->StartCoroutine(Organize());
    }

    GameObject *main_buttons = GameObject::Find(il2cpp_utils::createcsstr("MainButtons"));
    getLogger().info("MainButtons pointer: %p", main_buttons);

    Transform *campaign = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("CampaignButton"));
    Transform *solo = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("SoloButton"));
    Transform *online = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("OnlineButton"));
    Transform *party = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("PartyButton"));
    Transform *options = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("OptionsButton"));
    Transform *help = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("HelpButton"));
    Transform *exit = main_buttons->get_transform()->Find(il2cpp_utils::createcsstr("ExitButton"));

}

extern "C" void setup(ModInfo &info) {
    info.id = "BetterMenu";
    info.version = "0.1.0";
    modInfo = info;
}

extern "C" void load() {
    getLogger().info("Installing BetterMenu Hooks!");

    il2cpp_functions::Init();

    auto on_load = il2cpp_utils::MakeAction<UnityAction_2<UnityEngine::SceneManagement::Scene, LoadSceneMode>>(
        CRASH_UNLESS(il2cpp_functions::class_get_type(
            il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_class<UnityAction_2<UnityEngine::SceneManagement::Scene, LoadSceneMode> *>::get())),
        (void*)nullptr, OnLoad);
    SceneManager::add_sceneLoaded(on_load);

    getLogger().info("Installed BetterMenu Hooks!");
}