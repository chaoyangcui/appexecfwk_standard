/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include <fstream>

#include "directory_ex.h"
#include "bundle_data_mgr.h"
#include "install_param.h"
#include "bundle_mgr_service.h"
#include "bundle_mgr_host.h"
#include "bundle_info.h"
#include "installd/installd_service.h"
#include "installd_client.h"
#include "inner_bundle_info.h"
#include "mock_clean_cache.h"
#include "mock_bundle_status.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::AppExecFwk;
using OHOS::AAFwk::Want;

namespace {

const std::string BUNDLE_NAME_TEST = "com.example.bundlekit.test";
const std::string MODULE_NAME_TEST = "com.example.bundlekit.test.entry";
const std::string ABILITY_NAME_TEST = ".Reading";
const int TEST_UID = 1001;
const std::string BUNDLE_LABEL = "Hello, OHOS";
const std::string BUNDLE_DESCRIPTION = "example helloworld";
const std::string BUNDLE_VENDOR = "example";
const std::string BUNDLE_VERSION_NAME = "1.0.0.1";
const std::string BUNDLE_MAIN_ABILITY = "com.example.bundlekit.test.entry";
const int32_t BUNDLE_MAX_SDK_VERSION = 0;
const int32_t BUNDLE_MIN_SDK_VERSION = 0;
const std::string BUNDLE_JOINT_USERID = "3";
const uint32_t BUNDLE_VERSION_CODE = 1001;
const std::string BUNDLE_NAME_DEMO = "com.example.bundlekit.demo";
const std::string MODULE_NAME_DEMO = "com.example.bundlekit.demo.entry";
const std::string ABILITY_NAME_DEMO = ".Writing";
const int DEMO_UID = 30001;
const std::string PACKAGE_NAME = "com.example.bundlekit.test.entry";
const std::string PROCESS_TEST = "test.process";
const std::string DEVICE_ID = "PHONE-001";
const int APPLICATION_INFO_FLAGS = 1;
const int DEFAULT_USER_ID = 0;
const std::string LABEL = "hello";
const std::string DESCRIPTION = "mainEntry";
const std::string THEME = "mytheme";
const std::string ICON_PATH = "/data/data/icon.png";
const std::string KIND = "test";
const std::string ACTION = "action.system.home";
const std::string ENTITY = "entity.system.home";
const std::string URI_SCHEME = "fakescheme";
const std::string URI_HOST = "fakehost";
const std::string TARGET_ABILITY = "MockTargetAbility";
const AbilityType ABILITY_TYPE = AbilityType::PAGE;
const DisplayOrientation ORIENTATION = DisplayOrientation::PORTRAIT;
const LaunchMode LAUNCH_MODE = LaunchMode::SINGLETON;
const uint32_t FORM_ENTITY = 1;
const int DEFAULT_FORM_HEIGHT = 100;
const int DEFAULT_FORM_WIDTH = 200;
const std::string META_DATA_DESCRIPTION = "description";
const std::string META_DATA_NAME = "name";
const std::string META_DATA_TYPE = "type";
const std::string META_DATA_VALUE = "value";
const std::string META_DATA_EXTRA = "extra";
const ModuleColorMode COLOR_MODE = ModuleColorMode::AUTO;
const std::string CODE_PATH = "/data/accounts/account_0/com.example.bundlekit.test/code";
const std::string RESOURCE_PATH = "/data/accounts/account_0/com.example.bundlekit.test/res";
const std::string LIB_PATH = "/data/accounts/account_0/com.example.bundlekit.test/lib";
const bool VISIBLE = true;
const std::string MAIN_ENTRY = "com.example.bundlekit.test.entry";
const uint32_t ABILITY_SIZE_ZERO = 0;
const uint32_t ABILITY_SIZE_ONE = 1;
const uint32_t PERMISSION_SIZE_ZERO = 0;
const uint32_t PERMISSION_SIZE_TWO = 2;
const uint32_t BUNDLE_NAMES_SIZE_ZERO = 0;
const uint32_t BUNDLE_NAMES_SIZE_ONE = 1;
const std::string EMPTY_STRING = "";
const int INVALID_UID = -1;
const std::string URI = "dataability://com.example.hiworld.himusic.UserADataAbility";
const std::string ERROR_URI = "dataability://";
const std::string HAP_FILE_PATH = "/data/test/resource/bms/bundle_kit/test.hap";
const std::string HAP_FILE_PATH1 = "/data/test/resource/bms/bundle_kit/test1.hap";
const std::string ERROR_HAP_FILE_PATH = "/data/test/resource/bms/bundle_kit/error.hap";
const std::string META_DATA = "name";
const std::string ERROR_META_DATA = "String";
const std::string BUNDLE_DATA_DIR = "/data/accounts/account_0/appdata/com.example.bundlekit.test";
const std::string FILES_DIR = "/data/accounts/account_0/appdata/com.example.bundlekit.test/files";
const std::string TEST_FILE_DIR = "/data/accounts/account_0/appdata/com.example.bundlekit.test/files/file";
const std::string DATA_BASE_DIR = "/data/accounts/account_0/appdata/com.example.bundlekit.test/database";
const std::string TEST_DATA_BASE_DIR = "/data/accounts/account_0/appdata/com.example.bundlekit.test/database/database";
const std::string CACHE_DIR = "/data/accounts/account_0/appdata/com.example.bundlekit.test/cache";
const std::string TEST_CACHE_DIR = "/data/accounts/account_0/appdata/com.example.bundlekit.test/cache/cache";
const std::string FORM_NAME = "form_js";
const std::string FORM_PATH = "data/app";
const std::string FORM_JS_COMPONENT_NAME = "JS";
const std::string FORM_DESCRIPTION = "description";
const std::string FORM_SCHEDULED_UPDATE_TIME = "11:00";
const std::string FORM_CUSTOMIZE_DATAS_NAME = "customizeDataName";
const std::string FORM_CUSTOMIZE_DATAS_VALUE = "customizeDataValue";
const std::string FORM_PORTRAIT_LAYOUTS1 = "port1";
const std::string FORM_PORTRAIT_LAYOUTS2 = "port2";
const std::string FORM_LANDSCAPE_LAYOUTS1 = "land1";
const std::string FORM_LANDSCAPE_LAYOUTS2 = "land2";
const std::string SHORTCUT_TEST_ID = "shortcutTestId";
const std::string SHORTCUT_DEMO_ID = "shortcutDemoId";
const std::string SHORTCUT_HOST_ABILITY = "hostAbility";
const std::string SHORTCUT_ICON = "/data/test/bms_bundle";
const std::string SHORTCUT_LABEL = "shortcutLabel";
const std::string SHORTCUT_DISABLE_MESSAGE = "shortcutDisableMessage";
const std::string SHORTCUT_INTENTS_TARGET_BUNDLE = "targetBundle";
const std::string SHORTCUT_INTENTS_TARGET_CLASS = "targetClass";
const int FORMINFO_DESCRIPTIONID = 123;

}  // namespace

class BmsBundleKitServiceTest : public testing::Test {
public:
    using Want = OHOS::AAFwk::Want;
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    std::shared_ptr<BundleDataMgr> GetBundleDataMgr() const;
    void MockInstallBundle(
        const std::string &bundleName, const std::string &moduleName, const std::string &abilityName) const;
    void MockUninstallBundle(const std::string &bundleName) const;
    AbilityInfo MockAbilityInfo(
        const std::string &bundleName, const std::string &module, const std::string &abilityName) const;
    FormInfo MockFormInfo(
        const std::string &bundleName, const std::string &module, const std::string &abilityName) const;
    ShortcutInfo MockShortcutInfo(const std::string &bundleName, const std::string &shortcutId) const;
    void CheckBundleInfo(const std::string &bundleName, const std::string &moduleName, const uint32_t abilitySize,
        const BundleInfo &bundleInfo) const;
    void CheckBundleArchiveInfo(const std::string &bundleName, const std::string &moduleName,
        const uint32_t abilitySize, const BundleInfo &bundleInfo) const;
    void CheckBundleList(const std::string &bundleName, const std::vector<std::string> &bundleList) const;
    void CheckApplicationInfo(
        const std::string &bundleName, const uint32_t permissionSize, const ApplicationInfo &appInfo) const;
    void CheckAbilityInfo(
        const std::string &bundleName, const std::string &abilityName, const AbilityInfo &appInfo) const;
    void CheckCompatibleApplicationInfo(
        const std::string &bundleName, const uint32_t permissionSize, const CompatibleApplicationInfo &appInfo) const;
    void CheckCompatibleAbilityInfo(
        const std::string &bundleName, const std::string &abilityName, const CompatibleAbilityInfo &appInfo) const;
    void CheckInstalledBundleInfos(const uint32_t abilitySize, const std::vector<BundleInfo> &bundleInfos) const;
    void CheckInstalledApplicationInfos(const uint32_t permsSize, const std::vector<ApplicationInfo> &appInfos) const;
    void CheckModuleInfo(const HapModuleInfo &hapModuleInfo) const;
    void CreateFileDir() const;
    void CleanFileDir() const;
    void CheckFileExist() const;
    void CheckFileNonExist() const;
    void CheckCacheExist() const;
    void CheckCacheNonExist() const;
    void CheckFormInfoTest(const std::vector<FormInfo> &forms) const;
    void CheckFormInfoDemo(const std::vector<FormInfo> &forms) const;
    void CheckShortcutInfoTest(std::vector<ShortcutInfo> &shortcutInfos) const;
    void CheckShortcutInfoDemo(std::vector<ShortcutInfo> &shortcutInfos) const;

public:
    std::shared_ptr<BundleMgrService> bundleMgrService_ = DelayedSingleton<BundleMgrService>::GetInstance();
    std::shared_ptr<InstalldService> service_ = std::make_shared<InstalldService>();
};

void BmsBundleKitServiceTest::SetUpTestCase()
{}

void BmsBundleKitServiceTest::TearDownTestCase()
{}

void BmsBundleKitServiceTest::SetUp()
{
    bundleMgrService_->OnStart();
}

void BmsBundleKitServiceTest::TearDown()
{}

std::shared_ptr<BundleDataMgr> BmsBundleKitServiceTest::GetBundleDataMgr() const
{
    return bundleMgrService_->GetDataMgr();
}

void BmsBundleKitServiceTest::MockInstallBundle(
    const std::string &bundleName, const std::string &moduleName, const std::string &abilityName) const
{
    InnerBundleInfo innerBundleInfo;
    ApplicationInfo appInfo;
    appInfo.bundleName = bundleName;
    appInfo.name = bundleName;
    appInfo.deviceId = DEVICE_ID;
    appInfo.process = PROCESS_TEST;
    appInfo.label = BUNDLE_LABEL;
    appInfo.description = BUNDLE_DESCRIPTION;
    appInfo.codePath = CODE_PATH;
    appInfo.dataDir = FILES_DIR;
    appInfo.dataBaseDir = DATA_BASE_DIR;
    appInfo.cacheDir = CACHE_DIR;
    appInfo.flags = APPLICATION_INFO_FLAGS;
    appInfo.enabled = true;

    BundleInfo bundleInfo;
    bundleInfo.name = bundleName;
    bundleInfo.label = BUNDLE_LABEL;
    bundleInfo.description = BUNDLE_DESCRIPTION;
    bundleInfo.vendor = BUNDLE_VENDOR;
    bundleInfo.versionCode = BUNDLE_VERSION_CODE;
    bundleInfo.versionName = BUNDLE_VERSION_NAME;
    bundleInfo.minSdkVersion = BUNDLE_MIN_SDK_VERSION;
    bundleInfo.maxSdkVersion = BUNDLE_MAX_SDK_VERSION;
    bundleInfo.mainEntry = MAIN_ENTRY;
    bundleInfo.isKeepAlive = true;
    bundleInfo.isDifferentName = true;
    bundleInfo.jointUserId = BUNDLE_JOINT_USERID;

    InnerModuleInfo moduleInfo;
    ReqPermission reqPermission1 = {.name = "permission1"};
    ReqPermission reqPermission2 = {.name = "permission1"};
    moduleInfo.reqPermissions = {reqPermission1, reqPermission2};
    moduleInfo.modulePackage = PACKAGE_NAME;
    moduleInfo.moduleName = PACKAGE_NAME;
    moduleInfo.description = BUNDLE_DESCRIPTION;
    moduleInfo.colorMode = COLOR_MODE;

    AppExecFwk::Parameters parameters{"description", "name", "type"};
    AppExecFwk::Results results{"description", "name", "type"};
    AppExecFwk::CustomizeData customizeData{"name", "value", "extra"};
    MetaData metaData{{parameters}, {results}, {customizeData}};
    moduleInfo.metaData = metaData;

    FormInfo form = MockFormInfo(bundleName, moduleName, abilityName);
    AbilityInfo abilityInfo = MockAbilityInfo(bundleName, moduleName, abilityName);
    innerBundleInfo.SetBaseApplicationInfo(appInfo);
    innerBundleInfo.SetBaseBundleInfo(bundleInfo);
    innerBundleInfo.InsertInnerModuleInfo(moduleName, moduleInfo);
    std::string keyName = bundleName + moduleName + abilityName;
    innerBundleInfo.InsertAbilitiesInfo(keyName, abilityInfo);
    innerBundleInfo.SetUid((bundleName == BUNDLE_NAME_TEST) ? TEST_UID : DEMO_UID);
    // for launch ability
    if (bundleName == BUNDLE_NAME_TEST) {
        AppExecFwk::SkillUri uri{URI_SCHEME, URI_HOST};
        Skill skill{{ACTION}, {ENTITY}, {uri}};
        std::vector<Skill> skills;
        skills.emplace_back(skill);
        innerBundleInfo.SetMainAbility(keyName);
        innerBundleInfo.InsertSkillInfo(keyName, skills);
    }
    std::vector<FormInfo> formInfos;
    formInfos.emplace_back(form);
    if(bundleName == BUNDLE_NAME_TEST) {
        ShortcutInfo shortcut = MockShortcutInfo(bundleName, SHORTCUT_TEST_ID);
        std::string shortcutKey = bundleName + moduleName + SHORTCUT_TEST_ID;
        innerBundleInfo.InsertShortcutInfos(shortcutKey, shortcut);
    }
    else {
        ShortcutInfo shortcut = MockShortcutInfo(bundleName, SHORTCUT_DEMO_ID);
        std::string shortcutKey = bundleName + moduleName + SHORTCUT_DEMO_ID;
        innerBundleInfo.InsertShortcutInfos(shortcutKey, shortcut);
    }
    innerBundleInfo.InsertFormInfos(keyName, formInfos);
    auto dataMgr = GetBundleDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool startRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::INSTALL_START);
    bool finishRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::INSTALL_SUCCESS);
    bool addRet = dataMgr->AddInnerBundleInfo(bundleName, innerBundleInfo);

    ASSERT_TRUE(startRet);
    ASSERT_TRUE(finishRet);
    ASSERT_TRUE(addRet);
}

FormInfo BmsBundleKitServiceTest::MockFormInfo(
    const std::string &bundleName, const std::string &moduleName, const std::string &abilityName) const
{
    FormInfo formInfo;
    formInfo.name = FORM_NAME;
    formInfo.bundleName = bundleName;
    formInfo.abilityName = abilityName;
    formInfo.moduleName = moduleName;
    formInfo.package = PACKAGE_NAME;
    formInfo.descriptionId = FORMINFO_DESCRIPTIONID;
    formInfo.formConfigAbility = FORM_PATH;
    formInfo.description = FORM_DESCRIPTION;
    formInfo.defaultFlag = false;
    formInfo.type = FormType::JS;
    formInfo.colorMode = FormsColorMode::LIGHT_MODE;
    formInfo.supportDimensions = {1, 2};
    formInfo.portraitLayouts = {FORM_PORTRAIT_LAYOUTS1, FORM_PORTRAIT_LAYOUTS2};
    formInfo.landscapeLayouts = {FORM_LANDSCAPE_LAYOUTS1, FORM_LANDSCAPE_LAYOUTS2};
    formInfo.defaultDimension = 1;
    formInfo.updateDuration = 0;
    formInfo.formVisibleNotify = true;
    formInfo.deepLink = FORM_PATH;
    formInfo.scheduledUpateTime = FORM_SCHEDULED_UPDATE_TIME;
    formInfo.updateEnabled = true;
    formInfo.jsComponentName = FORM_JS_COMPONENT_NAME;
    for (auto &info : formInfo.customizeDatas) {
        info.name = FORM_CUSTOMIZE_DATAS_NAME;
        info.value = FORM_CUSTOMIZE_DATAS_VALUE;
    }
    return formInfo;
}

ShortcutInfo BmsBundleKitServiceTest::MockShortcutInfo(const std::string &bundleName, const std::string &shortcutId) const
{
    ShortcutInfo shortcutInfos;
    shortcutInfos.id = shortcutId;
    shortcutInfos.bundleName = bundleName;
    shortcutInfos.hostAbility = SHORTCUT_HOST_ABILITY;
    shortcutInfos.icon = SHORTCUT_ICON;
    shortcutInfos.label = SHORTCUT_LABEL;
    shortcutInfos.disableMessage = SHORTCUT_DISABLE_MESSAGE;
    shortcutInfos.isStatic = true;
    shortcutInfos.isHomeShortcut = true;
    shortcutInfos.isEnables = true;
    for (auto &info : shortcutInfos.intents) {
        info.targetBundle = SHORTCUT_INTENTS_TARGET_BUNDLE;
        info.targetClass = SHORTCUT_INTENTS_TARGET_CLASS;
    }
    return shortcutInfos;
}

void BmsBundleKitServiceTest::MockUninstallBundle(const std::string &bundleName) const
{
    auto dataMgr = GetBundleDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool startRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::UNINSTALL_START);
    bool finishRet = dataMgr->UpdateBundleInstallState(bundleName, InstallState::UNINSTALL_SUCCESS);

    ASSERT_TRUE(startRet);
    ASSERT_TRUE(finishRet);
}

AbilityInfo BmsBundleKitServiceTest::MockAbilityInfo(
    const std::string &bundleName, const std::string &moduleName, const std::string &abilityName) const
{
    AbilityInfo abilityInfo;
    abilityInfo.package = PACKAGE_NAME;
    abilityInfo.name = abilityName;
    abilityInfo.bundleName = bundleName;
    abilityInfo.moduleName = moduleName;
    abilityInfo.deviceId = DEVICE_ID;
    abilityInfo.label = LABEL;
    abilityInfo.description = DESCRIPTION;
    abilityInfo.theme = THEME;
    abilityInfo.iconPath = ICON_PATH;
    abilityInfo.visible = VISIBLE;
    abilityInfo.kind = KIND;
    abilityInfo.type = ABILITY_TYPE;
    abilityInfo.orientation = ORIENTATION;
    abilityInfo.launchMode = LAUNCH_MODE;
    abilityInfo.formEntity = 1;
    abilityInfo.defaultFormHeight = DEFAULT_FORM_HEIGHT;
    abilityInfo.defaultFormWidth = DEFAULT_FORM_WIDTH;
    abilityInfo.codePath = CODE_PATH;
    abilityInfo.resourcePath = RESOURCE_PATH;
    abilityInfo.libPath = LIB_PATH;
    abilityInfo.uri = URI;
    abilityInfo.enabled = true;
    abilityInfo.targetAbility = TARGET_ABILITY;
    AppExecFwk::Parameters parameters{"description", "name", "type"};
    AppExecFwk::Results results{"description", "name", "type"};
    AppExecFwk::CustomizeData customizeData{"name", "value", "extra"};
    MetaData metaData{{parameters}, {results}, {customizeData}};
    abilityInfo.metaData = metaData;
    return abilityInfo;
}

void BmsBundleKitServiceTest::CheckBundleInfo(const std::string &bundleName, const std::string &moduleName,
    const uint32_t abilitySize, const BundleInfo &bundleInfo) const
{
    EXPECT_EQ(bundleName, bundleInfo.name);
    EXPECT_EQ(BUNDLE_LABEL, bundleInfo.label);
    EXPECT_EQ(BUNDLE_DESCRIPTION, bundleInfo.description);
    EXPECT_EQ(BUNDLE_VENDOR, bundleInfo.vendor);
    EXPECT_EQ(BUNDLE_VERSION_CODE, bundleInfo.versionCode);
    EXPECT_EQ(BUNDLE_VERSION_NAME, bundleInfo.versionName);
    EXPECT_EQ(BUNDLE_MIN_SDK_VERSION, bundleInfo.minSdkVersion);
    EXPECT_EQ(BUNDLE_MAX_SDK_VERSION, bundleInfo.maxSdkVersion);
    EXPECT_EQ(BUNDLE_MAIN_ABILITY, bundleInfo.mainEntry);
    EXPECT_EQ(bundleName, bundleInfo.applicationInfo.name);
    EXPECT_EQ(bundleName, bundleInfo.applicationInfo.bundleName);
    EXPECT_EQ(abilitySize, static_cast<uint32_t>(bundleInfo.abilityInfos.size()));
    EXPECT_EQ(true, bundleInfo.isDifferentName);
    EXPECT_EQ(BUNDLE_JOINT_USERID, bundleInfo.jointUserId);
}

void BmsBundleKitServiceTest::CheckBundleArchiveInfo(const std::string &bundleName, const std::string &moduleName,
    const uint32_t abilitySize, const BundleInfo &bundleInfo) const
{
    EXPECT_EQ(bundleName, bundleInfo.name);
    EXPECT_EQ(BUNDLE_LABEL, bundleInfo.label);
    EXPECT_EQ(BUNDLE_DESCRIPTION, bundleInfo.description);
    EXPECT_EQ(BUNDLE_VENDOR, bundleInfo.vendor);
    EXPECT_EQ(BUNDLE_VERSION_CODE, bundleInfo.versionCode);
    EXPECT_EQ(BUNDLE_VERSION_NAME, bundleInfo.versionName);
    EXPECT_EQ(BUNDLE_MIN_SDK_VERSION, bundleInfo.minSdkVersion);
    EXPECT_EQ(BUNDLE_MAX_SDK_VERSION, bundleInfo.maxSdkVersion);
    EXPECT_EQ(BUNDLE_MAIN_ABILITY, bundleInfo.mainEntry);
    EXPECT_EQ(bundleName, bundleInfo.applicationInfo.name);
    EXPECT_EQ(bundleName, bundleInfo.applicationInfo.bundleName);
    EXPECT_EQ(abilitySize, static_cast<uint32_t>(bundleInfo.abilityInfos.size()));
}

void BmsBundleKitServiceTest::CheckBundleList(
    const std::string &bundleName, const std::vector<std::string> &bundleList) const
{
    EXPECT_TRUE(std::find(bundleList.begin(), bundleList.end(), bundleName) != bundleList.end());
}

void BmsBundleKitServiceTest::CheckApplicationInfo(
    const std::string &bundleName, const uint32_t permissionSize, const ApplicationInfo &appInfo) const
{
    EXPECT_EQ(bundleName, appInfo.name);
    EXPECT_EQ(bundleName, appInfo.bundleName);
    EXPECT_EQ(BUNDLE_LABEL, appInfo.label);
    EXPECT_EQ(BUNDLE_DESCRIPTION, appInfo.description);
    EXPECT_EQ(DEVICE_ID, appInfo.deviceId);
    EXPECT_EQ(PROCESS_TEST, appInfo.process);
    EXPECT_EQ(CODE_PATH, appInfo.codePath);
    EXPECT_EQ(permissionSize, static_cast<uint32_t>(appInfo.permissions.size()));
    EXPECT_EQ(APPLICATION_INFO_FLAGS, appInfo.flags);
}

void BmsBundleKitServiceTest::CheckAbilityInfo(
    const std::string &bundleName, const std::string &abilityName, const AbilityInfo &abilityInfo) const
{
    EXPECT_EQ(abilityName, abilityInfo.name);
    EXPECT_EQ(bundleName, abilityInfo.bundleName);
    EXPECT_EQ(LABEL, abilityInfo.label);
    EXPECT_EQ(DESCRIPTION, abilityInfo.description);
    EXPECT_EQ(DEVICE_ID, abilityInfo.deviceId);
    EXPECT_EQ(THEME, abilityInfo.theme);
    EXPECT_EQ(ICON_PATH, abilityInfo.iconPath);
    EXPECT_EQ(CODE_PATH, abilityInfo.codePath);
    EXPECT_EQ(ORIENTATION, abilityInfo.orientation);
    EXPECT_EQ(LAUNCH_MODE, abilityInfo.launchMode);
    EXPECT_EQ(URI, abilityInfo.uri);
    EXPECT_EQ(TARGET_ABILITY, abilityInfo.targetAbility);
    EXPECT_EQ(FORM_ENTITY, abilityInfo.formEntity);
    EXPECT_EQ(DEFAULT_FORM_HEIGHT, abilityInfo.defaultFormHeight);
    EXPECT_EQ(DEFAULT_FORM_WIDTH, abilityInfo.defaultFormWidth);
    for (auto &info : abilityInfo.metaData.customizeData) {
            EXPECT_EQ(info.name, META_DATA_NAME);
            EXPECT_EQ(info.value, META_DATA_VALUE);
            EXPECT_EQ(info.extra, META_DATA_EXTRA);
        }
    for (auto &info : abilityInfo.metaData.parameters) {
            EXPECT_EQ(info.description, META_DATA_DESCRIPTION);
            EXPECT_EQ(info.name, META_DATA_NAME);
            EXPECT_EQ(info.type, META_DATA_TYPE);
        }
    for (auto &info : abilityInfo.metaData.results) {
            EXPECT_EQ(info.description, META_DATA_DESCRIPTION);
            EXPECT_EQ(info.name, META_DATA_NAME);
            EXPECT_EQ(info.type, META_DATA_TYPE);
        }
}

void BmsBundleKitServiceTest::CheckCompatibleAbilityInfo(
    const std::string &bundleName, const std::string &abilityName, const CompatibleAbilityInfo &abilityInfo) const
{
    EXPECT_EQ(abilityName, abilityInfo.name);
    EXPECT_EQ(bundleName, abilityInfo.bundleName);
    EXPECT_EQ(LABEL, abilityInfo.label);
    EXPECT_EQ(DESCRIPTION, abilityInfo.description);
    EXPECT_EQ(DEVICE_ID, abilityInfo.deviceId);
    EXPECT_EQ(ICON_PATH, abilityInfo.iconPath);
    EXPECT_EQ(ORIENTATION, abilityInfo.orientation);
    EXPECT_EQ(LAUNCH_MODE, abilityInfo.launchMode);
    EXPECT_EQ(URI, abilityInfo.uri);
    EXPECT_EQ(false, abilityInfo.supportPipMode);
    EXPECT_EQ(TARGET_ABILITY, abilityInfo.targetAbility);
    EXPECT_EQ(FORM_ENTITY, abilityInfo.formEntity);
    EXPECT_EQ(DEFAULT_FORM_HEIGHT, abilityInfo.defaultFormHeight);
    EXPECT_EQ(DEFAULT_FORM_WIDTH, abilityInfo.defaultFormWidth);

}
void BmsBundleKitServiceTest::CheckCompatibleApplicationInfo(
    const std::string &bundleName, const uint32_t permissionSize, const CompatibleApplicationInfo &appInfo) const
{
    EXPECT_EQ(bundleName, appInfo.name);
    EXPECT_EQ(BUNDLE_LABEL, appInfo.label);
    EXPECT_EQ(BUNDLE_DESCRIPTION, appInfo.description);
    EXPECT_EQ(PROCESS_TEST, appInfo.process);
    EXPECT_EQ(permissionSize, static_cast<uint32_t>(appInfo.permissions.size()));
}

void BmsBundleKitServiceTest::CheckModuleInfo(const HapModuleInfo &hapModuleInfo) const
{
    EXPECT_EQ(MODULE_NAME_TEST, hapModuleInfo.name);
    EXPECT_EQ(MODULE_NAME_TEST, hapModuleInfo.moduleName);
    EXPECT_EQ(BUNDLE_DESCRIPTION, hapModuleInfo.description);
    EXPECT_EQ(ICON_PATH, hapModuleInfo.iconPath);
    EXPECT_EQ(LABEL, hapModuleInfo.label);
    EXPECT_EQ(COLOR_MODE, hapModuleInfo.colorMode);
}

void BmsBundleKitServiceTest::CheckInstalledBundleInfos(
    const uint32_t abilitySize, const std::vector<BundleInfo> &bundleInfos) const
{
    bool isContainsDemoBundle = false;
    bool isContainsTestBundle = false;
    bool checkDemoAppNameRet = false;
    bool checkTestAppNameRet = false;
    bool checkDemoAbilitySizeRet = false;
    bool checkTestAbilitySizeRet = false;
    for (auto item : bundleInfos) {
        if (item.name == BUNDLE_NAME_DEMO) {
            isContainsDemoBundle = true;
            checkDemoAppNameRet = item.applicationInfo.name == BUNDLE_NAME_DEMO;
            uint32_t num = static_cast<uint32_t>(item.abilityInfos.size());
            checkDemoAbilitySizeRet = num == abilitySize;
        }
        if (item.name == BUNDLE_NAME_TEST) {
            isContainsTestBundle = true;
            checkTestAppNameRet = item.applicationInfo.name == BUNDLE_NAME_TEST;
            uint32_t num = static_cast<uint32_t>(item.abilityInfos.size());
            checkTestAbilitySizeRet = num == abilitySize;
        }
    }
    EXPECT_TRUE(isContainsDemoBundle);
    EXPECT_TRUE(isContainsTestBundle);
    EXPECT_TRUE(checkDemoAppNameRet);
    EXPECT_TRUE(checkTestAppNameRet);
    EXPECT_TRUE(checkDemoAbilitySizeRet);
    EXPECT_TRUE(checkTestAbilitySizeRet);
}

void BmsBundleKitServiceTest::CheckInstalledApplicationInfos(
    const uint32_t permsSize, const std::vector<ApplicationInfo> &appInfos) const
{
    bool isContainsDemoBundle = false;
    bool isContainsTestBundle = false;
    bool checkDemoAppNameRet = false;
    bool checkTestAppNameRet = false;
    bool checkDemoAbilitySizeRet = false;
    bool checkTestAbilitySizeRet = false;
    for (auto item : appInfos) {
        if (item.name == BUNDLE_NAME_DEMO) {
            isContainsDemoBundle = true;
            checkDemoAppNameRet = item.bundleName == BUNDLE_NAME_DEMO;
            uint32_t num = static_cast<uint32_t>(item.permissions.size());
            checkDemoAbilitySizeRet = num == permsSize;
        }
        if (item.name == BUNDLE_NAME_TEST) {
            isContainsTestBundle = true;
            checkTestAppNameRet = item.bundleName == BUNDLE_NAME_TEST;
            uint32_t num = static_cast<uint32_t>(item.permissions.size());
            checkTestAbilitySizeRet = num == permsSize;
        }
    }
    EXPECT_TRUE(isContainsDemoBundle);
    EXPECT_TRUE(isContainsTestBundle);
    EXPECT_TRUE(checkDemoAppNameRet);
    EXPECT_TRUE(checkTestAppNameRet);
    EXPECT_TRUE(checkDemoAbilitySizeRet);
    EXPECT_TRUE(checkTestAbilitySizeRet);
}

void BmsBundleKitServiceTest::CreateFileDir() const
{
    if (!service_->IsServiceReady()) {
        service_->Start();
    }

    if (access(TEST_FILE_DIR.c_str(), F_OK) != 0) {
        bool result = OHOS::ForceCreateDirectory(TEST_FILE_DIR);
        ASSERT_TRUE(result) << "fail to create file dir";
    }

    if (access(TEST_CACHE_DIR.c_str(), F_OK) != 0) {
        bool result = OHOS::ForceCreateDirectory(TEST_CACHE_DIR);
        ASSERT_TRUE(result) << "fail to create cache dir";
    }
}

void BmsBundleKitServiceTest::CleanFileDir() const
{
    service_->Stop();
    InstalldClient::GetInstance()->ResetInstalldProxy();

    OHOS::ForceRemoveDirectory(BUNDLE_DATA_DIR);
}

void BmsBundleKitServiceTest::CheckFileExist() const
{
    int dataExist = access(TEST_FILE_DIR.c_str(), F_OK);
    ASSERT_EQ(dataExist, 0);
}

void BmsBundleKitServiceTest::CheckFileNonExist() const
{
    int dataExist = access(TEST_FILE_DIR.c_str(), F_OK);
    ASSERT_NE(dataExist, 0);
}

void BmsBundleKitServiceTest::CheckCacheExist() const
{
    int dataExist = access(TEST_CACHE_DIR.c_str(), F_OK);
    ASSERT_EQ(dataExist, 0);
}

void BmsBundleKitServiceTest::CheckCacheNonExist() const
{
    int dataExist = access(TEST_CACHE_DIR.c_str(), F_OK);
    ASSERT_NE(dataExist, 0);
}

void BmsBundleKitServiceTest::CheckFormInfoTest(const std::vector<FormInfo> &formInfos) const
{
    for (auto &formInfo : formInfos) {
        EXPECT_EQ(formInfo.name, FORM_NAME);
        EXPECT_EQ(formInfo.bundleName, BUNDLE_NAME_TEST);
        EXPECT_EQ(formInfo.moduleName, MODULE_NAME_TEST);
        EXPECT_EQ(formInfo.abilityName, ABILITY_NAME_TEST);
        EXPECT_EQ(formInfo.description, FORM_DESCRIPTION);
        EXPECT_EQ(formInfo.formConfigAbility, FORM_PATH);
        EXPECT_EQ(formInfo.defaultFlag, false);
        EXPECT_EQ(formInfo.type, FormType::JS);
        EXPECT_EQ(formInfo.colorMode, FormsColorMode::LIGHT_MODE);
        EXPECT_EQ(formInfo.descriptionId, FORMINFO_DESCRIPTIONID);
        EXPECT_EQ(formInfo.deepLink, FORM_PATH);
        EXPECT_EQ(formInfo.package, PACKAGE_NAME);
        EXPECT_EQ(formInfo.formVisibleNotify, true);
        unsigned i = 2;
        EXPECT_EQ(formInfo.supportDimensions.size(), i);
        EXPECT_EQ(formInfo.defaultDimension, 1);
        EXPECT_EQ(formInfo.updateDuration, 0);
        EXPECT_EQ(formInfo.scheduledUpateTime, FORM_SCHEDULED_UPDATE_TIME);
        EXPECT_EQ(formInfo.jsComponentName, FORM_JS_COMPONENT_NAME);
        EXPECT_EQ(formInfo.updateEnabled, true);
        for (auto &info : formInfo.customizeDatas) {
            EXPECT_EQ(info.name, FORM_CUSTOMIZE_DATAS_NAME);
            EXPECT_EQ(info.value, FORM_CUSTOMIZE_DATAS_VALUE);
        }
    }
}

void BmsBundleKitServiceTest::CheckFormInfoDemo(const std::vector<FormInfo> &formInfos) const
{
    for (const auto &formInfo : formInfos) {
        EXPECT_EQ(formInfo.name, FORM_NAME);
        EXPECT_EQ(formInfo.bundleName, BUNDLE_NAME_DEMO);
        EXPECT_EQ(formInfo.moduleName, MODULE_NAME_DEMO);
        EXPECT_EQ(formInfo.abilityName, ABILITY_NAME_TEST);
        EXPECT_EQ(formInfo.description, FORM_DESCRIPTION);
        EXPECT_EQ(formInfo.formConfigAbility, FORM_PATH);
        EXPECT_EQ(formInfo.defaultFlag, false);
        EXPECT_EQ(formInfo.type, FormType::JS);
        EXPECT_EQ(formInfo.colorMode, FormsColorMode::LIGHT_MODE);
        EXPECT_EQ(formInfo.descriptionId, FORMINFO_DESCRIPTIONID);
        EXPECT_EQ(formInfo.deepLink, FORM_PATH);
        EXPECT_EQ(formInfo.package, PACKAGE_NAME);
        EXPECT_EQ(formInfo.formVisibleNotify, true);
        unsigned i = 2;
        EXPECT_EQ(formInfo.supportDimensions.size(), i);
        EXPECT_EQ(formInfo.defaultDimension, 1);
        EXPECT_EQ(formInfo.updateDuration, 0);
        EXPECT_EQ(formInfo.scheduledUpateTime, FORM_SCHEDULED_UPDATE_TIME);
        EXPECT_EQ(formInfo.jsComponentName, FORM_JS_COMPONENT_NAME);
        EXPECT_EQ(formInfo.updateEnabled, true);
        for (auto &info : formInfo.customizeDatas) {
            EXPECT_EQ(info.name, FORM_CUSTOMIZE_DATAS_NAME);
            EXPECT_EQ(info.value, FORM_CUSTOMIZE_DATAS_VALUE);
        }
    }
}

void BmsBundleKitServiceTest::CheckShortcutInfoTest(std::vector<ShortcutInfo> &shortcutInfos) const
{
    for (const auto &shortcutInfo : shortcutInfos) {
        EXPECT_EQ(shortcutInfo.id, SHORTCUT_TEST_ID);
        EXPECT_EQ(shortcutInfo.bundleName, BUNDLE_NAME_TEST);
        EXPECT_EQ(shortcutInfo.hostAbility, SHORTCUT_HOST_ABILITY);
        EXPECT_EQ(shortcutInfo.icon, SHORTCUT_ICON);
        EXPECT_EQ(shortcutInfo.label, SHORTCUT_LABEL);
        EXPECT_EQ(shortcutInfo.disableMessage, SHORTCUT_DISABLE_MESSAGE);
        EXPECT_EQ(shortcutInfo.isStatic, true);
        EXPECT_EQ(shortcutInfo.isHomeShortcut, true);
        EXPECT_EQ(shortcutInfo.isEnables, true);
        for (auto &intent : shortcutInfo.intents) {
            EXPECT_EQ(intent.targetBundle, SHORTCUT_INTENTS_TARGET_BUNDLE);
            EXPECT_EQ(intent.targetClass, SHORTCUT_INTENTS_TARGET_CLASS);
        }
    }
}

void BmsBundleKitServiceTest::CheckShortcutInfoDemo(std::vector<ShortcutInfo> &shortcutInfos) const
{
    for (const auto &shortcutInfo : shortcutInfos) {
        EXPECT_EQ(shortcutInfo.id, SHORTCUT_DEMO_ID);
        EXPECT_EQ(shortcutInfo.bundleName, BUNDLE_NAME_DEMO);
        EXPECT_EQ(shortcutInfo.hostAbility, SHORTCUT_HOST_ABILITY);
        EXPECT_EQ(shortcutInfo.icon, SHORTCUT_ICON);
        EXPECT_EQ(shortcutInfo.label, SHORTCUT_LABEL);
        EXPECT_EQ(shortcutInfo.disableMessage, SHORTCUT_DISABLE_MESSAGE);
        EXPECT_EQ(shortcutInfo.isStatic, true);
        EXPECT_EQ(shortcutInfo.isHomeShortcut, true);
        EXPECT_EQ(shortcutInfo.isEnables, true);
        for (auto &intent : shortcutInfo.intents) {
            EXPECT_EQ(intent.targetBundle, SHORTCUT_INTENTS_TARGET_BUNDLE);
            EXPECT_EQ(intent.targetClass, SHORTCUT_INTENTS_TARGET_CLASS);
        }
    }
}

/**
 * @tc.number: GetBundleInfo_0100
 * @tc.name: test can get the bundleName's bundle info
 * @tc.desc: 1.system run normal
 *           2.get bundle info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfo_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    BundleInfo testResult;
    bool testRet = GetBundleDataMgr()->GetBundleInfo(BUNDLE_NAME_TEST, BundleFlag::GET_BUNDLE_DEFAULT, testResult);
    EXPECT_TRUE(testRet);
    CheckBundleInfo(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_SIZE_ZERO, testResult);
    BundleInfo demoResult;
    bool demoRet =
        GetBundleDataMgr()->GetBundleInfo(BUNDLE_NAME_DEMO, BundleFlag::GET_BUNDLE_WITH_ABILITIES, demoResult);
    EXPECT_TRUE(demoRet);
    CheckBundleInfo(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_SIZE_ONE, demoResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetBundleInfo_0200
 * @tc.name: test can not get the bundleName's bundle info which not exist in system
 * @tc.desc: 1.system run normal
 *           2.get bundle info failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfo_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    BundleInfo result;
    bool ret = GetBundleDataMgr()->GetBundleInfo(BUNDLE_NAME_DEMO, BundleFlag::GET_BUNDLE_DEFAULT, result);
    EXPECT_FALSE(ret);
    EXPECT_EQ(EMPTY_STRING, result.label);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundleInfo_0300
 * @tc.name: test can not get bundle info with empty bundle name
 * @tc.desc: 1.system run normal
 *           2.get bundle info failed with empty bundle name
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfo_0300, Function | SmallTest | Level0)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    BundleInfo result;
    bool ret = GetBundleDataMgr()->GetBundleInfo(EMPTY_STRING, BundleFlag::GET_BUNDLE_DEFAULT, result);
    EXPECT_FALSE(ret);
    EXPECT_EQ(EMPTY_STRING, result.name);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundleInfo_0400
 * @tc.name: test can not get the bundleName's bundle info with no bundle in system
 * @tc.desc: 1.system run normally and without any bundle
 *           2.get bundle info failed with no bundle in system
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfo_0400, Function | SmallTest | Level0)
{
    BundleInfo bundleInfo;
    bool ret = GetBundleDataMgr()->GetBundleInfo(BUNDLE_NAME_TEST, BundleFlag::GET_BUNDLE_DEFAULT, bundleInfo);
    ASSERT_FALSE(ret);
    EXPECT_EQ(EMPTY_STRING, bundleInfo.name);
    EXPECT_EQ(EMPTY_STRING, bundleInfo.label);
}

/**
 * @tc.number: GetBundleInfo_0500
 * @tc.name: test can parceable bundle info
 * @tc.desc: 1.system run normally
 *           2.get bundle info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfo_0500, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    BundleInfo bundleInfo;
    bool ret = GetBundleDataMgr()->GetBundleInfo(BUNDLE_NAME_TEST, BundleFlag::GET_BUNDLE_DEFAULT, bundleInfo);
    EXPECT_TRUE(ret);
    Parcel parcel;
    parcel.WriteParcelable(&bundleInfo);
    std::unique_ptr<BundleInfo> bundleInfoTest;
    bundleInfoTest.reset(parcel.ReadParcelable<BundleInfo>());
    EXPECT_EQ(bundleInfo.name, bundleInfoTest->name);
    EXPECT_EQ(bundleInfo.label, bundleInfoTest->label);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundleInfos_0100
 * @tc.name: test can get the installed bundles's bundle info with nomal flag
 * @tc.desc: 1.system run normally
 *           2.get all installed bundle info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfos_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<BundleInfo> bundleInfos;
    bool ret = GetBundleDataMgr()->GetBundleInfos(BundleFlag::GET_BUNDLE_DEFAULT, bundleInfos);
    ASSERT_TRUE(ret);
    CheckInstalledBundleInfos(ABILITY_SIZE_ZERO, bundleInfos);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetBundleInfos_0200
 * @tc.name: test can get the installed bundles's bundle info with abilities
 * @tc.desc: 1.system run normally
 *           2.get all installed bundle info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfos_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<BundleInfo> bundleInfos;
    bool ret = GetBundleDataMgr()->GetBundleInfos(BundleFlag::GET_BUNDLE_WITH_ABILITIES, bundleInfos);
    ASSERT_TRUE(ret);
    CheckInstalledBundleInfos(ABILITY_SIZE_ONE, bundleInfos);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetBundleInfos_0300
 * @tc.name: test can not get the installed bundles's bundle info with no bundle
 * @tc.desc: 1.system run normally
 *           2.get all installed bundle info failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfos_0300, Function | SmallTest | Level1)
{
    std::vector<BundleInfo> bundleInfos;
    bool ret = GetBundleDataMgr()->GetBundleInfos(BundleFlag::GET_BUNDLE_WITH_ABILITIES, bundleInfos);
    ASSERT_FALSE(ret);
}

/**
 * @tc.number: GetApplicationInfo_0100
 * @tc.name: test can get the appName's application info
 * @tc.desc: 1.system run normally
 *           2.get application info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetApplicationInfo_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);

    ApplicationInfo testResult;
    bool testRet = GetBundleDataMgr()->GetApplicationInfo(
        BUNDLE_NAME_TEST, ApplicationFlag::GET_BASIC_APPLICATION_INFO, DEFAULT_USER_ID, testResult);
    EXPECT_TRUE(testRet);
    CheckApplicationInfo(BUNDLE_NAME_TEST, PERMISSION_SIZE_ZERO, testResult);

    ApplicationInfo demoResult;
    bool demoRet = GetBundleDataMgr()->GetApplicationInfo(
        BUNDLE_NAME_DEMO, ApplicationFlag::GET_APPLICATION_INFO_WITH_PERMS, DEFAULT_USER_ID, demoResult);
    EXPECT_TRUE(demoRet);
    CheckApplicationInfo(BUNDLE_NAME_DEMO, PERMISSION_SIZE_TWO, demoResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetApplicationInfo_0200
 * @tc.name: test can not get the appName's application info which not exist in system
 * @tc.desc: 1.system run normally
 *           2.get application info failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetApplicationInfo_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    ApplicationInfo result;
    bool ret = GetBundleDataMgr()->GetApplicationInfo(
        BUNDLE_NAME_DEMO, ApplicationFlag::GET_BASIC_APPLICATION_INFO, DEFAULT_USER_ID, result);
    EXPECT_FALSE(ret);
    EXPECT_EQ(EMPTY_STRING, result.name);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetApplicationInfo_0300
 * @tc.name: test can not get application info with empty appName
 * @tc.desc: 1.system run normally
 *           2.get application info failed with empty appName
 */
HWTEST_F(BmsBundleKitServiceTest, GetApplicationInfo_0300, Function | SmallTest | Level0)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    ApplicationInfo result;
    bool ret = GetBundleDataMgr()->GetApplicationInfo(
        EMPTY_STRING, ApplicationFlag::GET_BASIC_APPLICATION_INFO, DEFAULT_USER_ID, result);
    EXPECT_FALSE(ret);
    EXPECT_EQ(EMPTY_STRING, result.name);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetApplicationInfo_0400
 * @tc.name: test can not get the appName's application info with no bundle in system
 * @tc.desc: 1.system run normally
 *           2.get application info failed with no bundle in system
 */
HWTEST_F(BmsBundleKitServiceTest, GetApplicationInfo_0400, Function | SmallTest | Level0)
{
    ApplicationInfo result;
    bool ret = GetBundleDataMgr()->GetApplicationInfo(
        BUNDLE_NAME_TEST, ApplicationFlag::GET_BASIC_APPLICATION_INFO, DEFAULT_USER_ID, result);
    ASSERT_FALSE(ret);
    EXPECT_EQ(EMPTY_STRING, result.name);
    EXPECT_EQ(EMPTY_STRING, result.label);
}

/**
 * @tc.number: GetApplicationInfo_0500
 * @tc.name: test can parceable application info
 * @tc.desc: 1.system run normally
 *           2.get application info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetApplicationInfo_0500, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    ApplicationInfo result;
    bool ret = GetBundleDataMgr()->GetApplicationInfo(
        BUNDLE_NAME_TEST, ApplicationFlag::GET_BASIC_APPLICATION_INFO, DEFAULT_USER_ID, result);
    EXPECT_TRUE(ret);
    Parcel parcel;
    parcel.WriteParcelable(&result);
    std::unique_ptr<ApplicationInfo> appInfoTest;
    appInfoTest.reset(parcel.ReadParcelable<ApplicationInfo>());
    EXPECT_EQ(result.name, appInfoTest->name);
    EXPECT_EQ(result.label, appInfoTest->label);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetApplicationInfos_0100
 * @tc.name: test can get the installed bundles's application info with basic info flag
 * @tc.desc: 1.system run normally
 *           2.get all installed application info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetApplicationInfos_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<ApplicationInfo> appInfos;
    bool ret =
        GetBundleDataMgr()->GetApplicationInfos(ApplicationFlag::GET_BASIC_APPLICATION_INFO, DEFAULT_USER_ID, appInfos);
    ASSERT_TRUE(ret);
    CheckInstalledApplicationInfos(PERMISSION_SIZE_ZERO, appInfos);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetApplicationInfos_0200
 * @tc.name: test can get the installed bundles's application info with permissions
 * @tc.desc: 1.system run normally
 *           2.get all installed application info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetApplicationInfos_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<ApplicationInfo> appInfos;
    bool ret = GetBundleDataMgr()->GetApplicationInfos(
        ApplicationFlag::GET_APPLICATION_INFO_WITH_PERMS, DEFAULT_USER_ID, appInfos);
    ASSERT_TRUE(ret);
    CheckInstalledApplicationInfos(PERMISSION_SIZE_TWO, appInfos);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetApplicationInfos_0300
 * @tc.name: test can not get the installed bundles's bundle info with no bundle
 * @tc.desc: 1.system run normally
 *           2.get all installed bundle info failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetApplicationInfos_0300, Function | SmallTest | Level1)
{
    std::vector<ApplicationInfo> appInfos;
    bool ret = GetBundleDataMgr()->GetApplicationInfos(
        ApplicationFlag::GET_APPLICATION_INFO_WITH_PERMS, DEFAULT_USER_ID, appInfos);
    ASSERT_FALSE(ret);
}

/**
 * @tc.number: GetAbilityLabel_0100
 * @tc.name: test can get the ability's label by bundleName and abilityName
 * @tc.desc: 1.system run normally
 *           2.get ability label successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetAbilityLabel_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string testRet = GetBundleDataMgr()->GetAbilityLabel(BUNDLE_NAME_TEST, ABILITY_NAME_TEST);
    EXPECT_EQ(LABEL, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetAbilityLabel_0200
 * @tc.name: test can not get the ability's label if bundle doesn't install
 * @tc.desc: 1.system run normally
 *           2.get empty ability label
 */
HWTEST_F(BmsBundleKitServiceTest, GetAbilityLabel_0200, Function | SmallTest | Level1)
{
    std::string testRet = GetBundleDataMgr()->GetAbilityLabel(BUNDLE_NAME_TEST, ABILITY_NAME_TEST);
    EXPECT_EQ(EMPTY_STRING, testRet);
}

/**
 * @tc.number: GetAbilityLabel_0300
 * @tc.name: test can not get the ability's label if bundle doesn't exist
 * @tc.desc: 1.system run normally
 *           2.get empty ability label
 */
HWTEST_F(BmsBundleKitServiceTest, GetAbilityLabel_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string testRet = GetBundleDataMgr()->GetAbilityLabel(BUNDLE_NAME_DEMO, ABILITY_NAME_TEST);
    EXPECT_EQ(EMPTY_STRING, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetAbilityLabel_0400
 * @tc.name: test can not get the ability's label if ability doesn't exist
 * @tc.desc: 1.system run normally
 *           2.get empty ability label
 */
HWTEST_F(BmsBundleKitServiceTest, GetAbilityLabel_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string testRet = GetBundleDataMgr()->GetAbilityLabel(BUNDLE_NAME_TEST, ABILITY_NAME_DEMO);
    EXPECT_EQ(EMPTY_STRING, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: QueryAbilityInfo_0100
 * @tc.name: test can get the ability info by want
 * @tc.desc: 1.system run normally
 *           2.get ability info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, QueryAbilityInfo_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    Want want;
    want.SetElementName(BUNDLE_NAME_TEST, ABILITY_NAME_TEST);
    AbilityInfo result;
    bool testRet = GetBundleDataMgr()->QueryAbilityInfo(want, result);
    EXPECT_EQ(true, testRet);
    CheckAbilityInfo(BUNDLE_NAME_TEST, ABILITY_NAME_TEST, result);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: QueryAbilityInfo_0200
 * @tc.name: test can not get the ability info by want in which element name is wrong
 * @tc.desc: 1.system run normally
 *           2.get ability info failed
 */
HWTEST_F(BmsBundleKitServiceTest, QueryAbilityInfo_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    Want want;
    want.SetElementName(BUNDLE_NAME_DEMO, ABILITY_NAME_TEST);
    AbilityInfo result;
    bool testRet = GetBundleDataMgr()->QueryAbilityInfo(want, result);
    EXPECT_EQ(false, testRet);

    want.SetElementName(BUNDLE_NAME_TEST, ABILITY_NAME_DEMO);
    testRet = GetBundleDataMgr()->QueryAbilityInfo(want, result);
    EXPECT_EQ(false, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: QueryAbilityInfo_0300
 * @tc.name: test can not get the ability info by want which bundle doesn't exist
 * @tc.desc: 1.system run normally
 *           2.get ability info failed
 */
HWTEST_F(BmsBundleKitServiceTest, QueryAbilityInfo_0300, Function | SmallTest | Level1)
{
    Want want;
    want.SetElementName(BUNDLE_NAME_TEST, ABILITY_NAME_TEST);
    AbilityInfo result;
    bool testRet = GetBundleDataMgr()->QueryAbilityInfo(want, result);
    EXPECT_EQ(false, testRet);
}

/**
 * @tc.number: GetLaunchWantForBundle_0100
 * @tc.name: test can get the launch want of a bundle
 * @tc.desc: 1.system run normally
 *           2.get launch want successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetLaunchWantForBundle_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    Want want;
    bool testRet = GetBundleDataMgr()->GetLaunchWantForBundle(BUNDLE_NAME_TEST, want);
    EXPECT_EQ(true, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetLaunchWantForBundle_0200
 * @tc.name: test can not get the launch want of a bundle which is not exist
 * @tc.desc: 1.system run normally
 *           2.get launch want failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetLaunchWantForBundle_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    Want want;
    bool testRet = GetBundleDataMgr()->GetLaunchWantForBundle(BUNDLE_NAME_DEMO, want);
    EXPECT_EQ(false, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetLaunchWantForBundle_0300
 * @tc.name: test can not get the launch want of a bundle which its mainability is not exist
 * @tc.desc: 1.system run normally
 *           2.get launch want failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetLaunchWantForBundle_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    Want want;
    bool testRet = GetBundleDataMgr()->GetLaunchWantForBundle(BUNDLE_NAME_DEMO, want);
    EXPECT_EQ(false, testRet);

    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetLaunchWantForBundle_0400
 * @tc.name: test can not get the launch want of empty name
 * @tc.desc: 1.system run normally
 *           2.get launch want failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetLaunchWantForBundle_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    Want want;
    bool testRet = GetBundleDataMgr()->GetLaunchWantForBundle(EMPTY_STRING, want);
    EXPECT_EQ(false, testRet);

    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetLaunchWantForBundle_0500
 * @tc.name: test can not get the launch want when no bundle installed
 * @tc.desc: 1.system run normally
 *           2.get launch want failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetLaunchWantForBundle_0500, Function | SmallTest | Level1)
{
    Want want;
    bool testRet = GetBundleDataMgr()->GetLaunchWantForBundle(BUNDLE_NAME_TEST, want);
    EXPECT_EQ(false, testRet);
}

/**
 * @tc.number: GetBundleList_0100
 * @tc.name: test can get all installed bundle names
 * @tc.desc: 1.system run normally
 *           2.get installed bundle names successfully with correct names
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleList_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);

    std::vector<std::string> testResult;
    bool testRet = GetBundleDataMgr()->GetBundleList(testResult);
    EXPECT_TRUE(testRet);
    CheckBundleList(BUNDLE_NAME_TEST, testResult);
    CheckBundleList(BUNDLE_NAME_DEMO, testResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetBundleList_0200
 * @tc.name: test can get the no bundle names with no bundle installed
 * @tc.desc: 1.system run normally
 *           2.get installed bundle names failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleList_0200, Function | SmallTest | Level1)
{
    std::vector<std::string> testResult;
    bool testRet = GetBundleDataMgr()->GetBundleList(testResult);
    EXPECT_FALSE(testRet);
}

/**
 * @tc.number: GetBundleNameForUid_0100
 * @tc.name: test can get the bundle names with bundle installed
 * @tc.desc: 1.system run normally
 *           2.get installed bundle names successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleNameForUid_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string testResult;
    bool testRet = GetBundleDataMgr()->GetBundleNameForUid(TEST_UID, testResult);
    EXPECT_TRUE(testRet);
    EXPECT_EQ(BUNDLE_NAME_TEST, testResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundleNameForUid_0200
 * @tc.name: test can not get not installed bundle name
 * @tc.desc: 1.system run normally
 *           2.get installed bundle name by uid failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleNameForUid_0200, Function | SmallTest | Level1)
{
    std::string testResult;
    bool testRet = GetBundleDataMgr()->GetBundleNameForUid(TEST_UID, testResult);
    EXPECT_FALSE(testRet);
    EXPECT_NE(BUNDLE_NAME_TEST, testResult);
}

/**
 * @tc.number: GetBundleNameForUid_0300
 * @tc.name: test can not get installed bundle name by incorrect uid
 * @tc.desc: 1.system run normally
 *           2.get installed bundle name by uid failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleNameForUid_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string testResult;
    bool testRet = GetBundleDataMgr()->GetBundleNameForUid(DEMO_UID, testResult);
    EXPECT_FALSE(testRet);
    EXPECT_NE(BUNDLE_NAME_TEST, testResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundleNameForUid_0400
 * @tc.name: test can not get installed bundle name by invalid uid
 * @tc.desc: 1.system run normally
 *           2.get installed bundle name by uid failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleNameForUid_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);

    std::string testResult;
    bool testRet = GetBundleDataMgr()->GetBundleNameForUid(INVALID_UID, testResult);
    EXPECT_FALSE(testRet);
    EXPECT_NE(BUNDLE_NAME_TEST, testResult);
    EXPECT_NE(BUNDLE_NAME_DEMO, testResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: CheckIsSystemAppByUid_0100
 * @tc.name: test can check the installed bundle whether system app or not by uid
 * @tc.desc: 1.system run normally
 *           2.check the installed bundle successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckIsSystemAppByUid_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);

    bool testRet = GetBundleDataMgr()->CheckIsSystemAppByUid(TEST_UID);
    EXPECT_TRUE(testRet);

    testRet = GetBundleDataMgr()->CheckIsSystemAppByUid(DEMO_UID);
    EXPECT_FALSE(testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: CheckIsSystemAppByUid_0200
 * @tc.name: test can check the installed bundle whether system app or not by uid
 * @tc.desc: 1.system run normally
 *           2.check the installed bundle successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckIsSystemAppByUid_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    bool testRet = GetBundleDataMgr()->CheckIsSystemAppByUid(INVALID_UID);
    EXPECT_FALSE(testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: DUMP_0100
 * @tc.name: Dump bundlelist, all bundle info, bundle info for bundleName
 * @tc.desc: 1.system run normally
 *           2.dump info with one mock installed bundles
 */
HWTEST_F(BmsBundleKitServiceTest, DUMP_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    std::string allInfoResult;
    bool allInfoRet = hostImpl->DumpInfos(DumpFlag::DUMP_ALL_BUNDLE_INFO, EMPTY_STRING, allInfoResult);
    EXPECT_TRUE(allInfoRet);
    EXPECT_NE(std::string::npos, allInfoResult.find(BUNDLE_NAME_TEST));
    EXPECT_NE(std::string::npos, allInfoResult.find(MODULE_NAME_TEST));
    EXPECT_NE(std::string::npos, allInfoResult.find(ABILITY_NAME_TEST));

    std::string infoResult;
    bool infoRet = hostImpl->DumpInfos(DumpFlag::DUMP_BUNDLE_INFO, BUNDLE_NAME_TEST, infoResult);
    EXPECT_TRUE(infoRet);
    EXPECT_NE(std::string::npos, infoResult.find(BUNDLE_NAME_TEST));
    EXPECT_NE(std::string::npos, infoResult.find(MODULE_NAME_TEST));
    EXPECT_NE(std::string::npos, infoResult.find(ABILITY_NAME_TEST));

    std::string bundleNames;
    bool listRet = hostImpl->DumpInfos(DumpFlag::DUMP_BUNDLE_LIST, EMPTY_STRING, bundleNames);
    EXPECT_TRUE(listRet);
    EXPECT_NE(std::string::npos, bundleNames.find(BUNDLE_NAME_TEST));

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_TEST, ABILITY_NAME_DEMO);

    std::string names;
    bool namesRet = hostImpl->DumpInfos(DumpFlag::DUMP_BUNDLE_LIST, EMPTY_STRING, names);
    EXPECT_TRUE(namesRet);
    EXPECT_NE(std::string::npos, names.find(BUNDLE_NAME_DEMO));

    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: DUMP_0200
 * @tc.name: Dump empty bundle info for empty bundle name
 * @tc.desc: 1.system run normally
 *           2.dump with empty bundle name
 */
HWTEST_F(BmsBundleKitServiceTest, DUMP_0200, Function | SmallTest | Level0)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    std::string emptyResult;
    bool emptyRet = hostImpl->DumpInfos(DumpFlag::DUMP_BUNDLE_INFO, EMPTY_STRING, emptyResult);
    EXPECT_FALSE(emptyRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: DUMP_0300
 * @tc.name: Dump bundlelist, all bundle info, bundle info for bundleName
 * @tc.desc: 1.system run normally
 *           2.dump info with 2 installed bundles
 */
HWTEST_F(BmsBundleKitServiceTest, DUMP_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    std::string bundleNames;
    bool listRet = hostImpl->DumpInfos(DumpFlag::DUMP_BUNDLE_LIST, EMPTY_STRING, bundleNames);
    EXPECT_TRUE(listRet);
    EXPECT_NE(std::string::npos, bundleNames.find(BUNDLE_NAME_DEMO));
    EXPECT_NE(std::string::npos, bundleNames.find(BUNDLE_NAME_TEST));

    std::string allBundleInfos;
    bool allInfoRet = hostImpl->DumpInfos(DumpFlag::DUMP_ALL_BUNDLE_INFO, EMPTY_STRING, allBundleInfos);
    EXPECT_TRUE(allInfoRet);
    EXPECT_NE(std::string::npos, allBundleInfos.find(BUNDLE_NAME_TEST));
    EXPECT_NE(std::string::npos, allBundleInfos.find(BUNDLE_NAME_DEMO));
    EXPECT_NE(std::string::npos, allBundleInfos.find(MODULE_NAME_TEST));
    EXPECT_NE(std::string::npos, allBundleInfos.find(MODULE_NAME_DEMO));
    EXPECT_NE(std::string::npos, allBundleInfos.find(ABILITY_NAME_TEST));
    EXPECT_NE(std::string::npos, allBundleInfos.find(ABILITY_NAME_DEMO));

    std::string bundleInfo;
    bool infoRet = hostImpl->DumpInfos(DumpFlag::DUMP_BUNDLE_INFO, BUNDLE_NAME_TEST, bundleInfo);
    EXPECT_TRUE(infoRet);
    EXPECT_NE(std::string::npos, allBundleInfos.find(BUNDLE_NAME_TEST));
    EXPECT_NE(std::string::npos, allBundleInfos.find(BUNDLE_NAME_DEMO));

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: DUMP_0400
 * @tc.name: Dump with no bundle in system
 * @tc.desc: 1.system run normally
 *           2.dump empty message with the dump command
 */
HWTEST_F(BmsBundleKitServiceTest, DUMP_0400, Function | SmallTest | Level0)
{
    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    std::string allBundleInfos;
    bool infoRet = hostImpl->DumpInfos(DumpFlag::DUMP_ALL_BUNDLE_INFO, EMPTY_STRING, allBundleInfos);
    EXPECT_FALSE(infoRet);
    EXPECT_EQ(std::string::npos, allBundleInfos.find(BUNDLE_NAME_TEST));

    std::string bundleInfo;
    bool infoRet1 = hostImpl->DumpInfos(DumpFlag::DUMP_BUNDLE_INFO, BUNDLE_NAME_TEST, bundleInfo);
    EXPECT_FALSE(infoRet1);
    EXPECT_EQ(EMPTY_STRING, bundleInfo);

    std::string emptyInfo;
    bool emptyRet = hostImpl->DumpInfos(DumpFlag::DUMP_BUNDLE_INFO, EMPTY_STRING, emptyInfo);
    EXPECT_FALSE(emptyRet);
    EXPECT_EQ(EMPTY_STRING, emptyInfo);
}

/**
 * @tc.number: QueryAbilityInfoByUri_0100
 * @tc.name: test can get the ability info by uri
 * @tc.desc: 1.system run normally
 *           2.get ability info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, QueryAbilityInfoByUri_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    AbilityInfo result;
    bool testRet = GetBundleDataMgr()->QueryAbilityInfoByUri(URI, result);
    EXPECT_EQ(true, testRet);
    EXPECT_EQ(ABILITY_NAME_TEST, result.name);
    EXPECT_NE(ABILITY_NAME_DEMO, result.name);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: QueryAbilityInfoByUri_0200
 * @tc.name: test can get the ability infos by uri
 * @tc.desc: 1.system run normally
 *           2.get ability info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, QueryAbilityInfoByUri_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    AbilityInfo result;
    bool testRet = GetBundleDataMgr()->QueryAbilityInfoByUri(URI, result);
    EXPECT_EQ(true, testRet);
    EXPECT_EQ(ABILITY_NAME_TEST, result.name);
    EXPECT_NE(ABILITY_NAME_DEMO, result.name);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: QueryAbilityInfoByUri_0300
 * @tc.name: test can not get the ability info by uri which bundle doesn't exist
 * @tc.desc: 1.system run normally
 *           2.get ability info failed
 */

HWTEST_F(BmsBundleKitServiceTest, QueryAbilityInfoByUri_0300, Function | SmallTest | Level1)
{
    AbilityInfo result;
    bool testRet = GetBundleDataMgr()->QueryAbilityInfoByUri(URI, result);
    EXPECT_EQ(false, testRet);
}

/**
 * @tc.number: QueryAbilityInfoByUri_0400
 * @tc.name: test can not get the ability info by empty uri
 * @tc.desc: 1.system run normally
 *           2.get ability info failed
 */
HWTEST_F(BmsBundleKitServiceTest, QueryAbilityInfoByUri_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    AbilityInfo result;
    bool testRet = GetBundleDataMgr()->QueryAbilityInfoByUri("", result);
    EXPECT_EQ(false, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: QueryAbilityInfoByUri_0500
 * @tc.name: test can not get the ability info by error uri
 * @tc.desc: 1.system run normally
 *           2.get ability info failed
 */
HWTEST_F(BmsBundleKitServiceTest, QueryAbilityInfoByUri_0500, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    AbilityInfo result;
    bool testRet = GetBundleDataMgr()->QueryAbilityInfoByUri(ERROR_URI, result);
    EXPECT_EQ(false, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: QueryKeepAliveBundleInfos_0100
 * @tc.name: test can get the keep alive bundle infos
 * @tc.desc: 1.system run normally
 *           2.get all keep alive bundle infos successfully
 */
HWTEST_F(BmsBundleKitServiceTest, QueryKeepAliveBundleInfos_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<BundleInfo> bundleInfos;
    bool ret = GetBundleDataMgr()->QueryKeepAliveBundleInfos(bundleInfos);
    EXPECT_EQ(true, ret);
    CheckInstalledBundleInfos(ABILITY_SIZE_ONE, bundleInfos);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: QueryKeepAliveBundleInfos_0200
 * @tc.name: test can not get the keep alive bundle info which bundle doesn't exist
 * @tc.desc: 1.system run normally
 *           2.get bundle info failed
 */
HWTEST_F(BmsBundleKitServiceTest, QueryKeepAliveBundleInfos_0200, Function | SmallTest | Level1)
{
    std::vector<BundleInfo> bundleInfos;
    bool ret = GetBundleDataMgr()->QueryKeepAliveBundleInfos(bundleInfos);
    EXPECT_EQ(false, ret);
}

/**
 * @tc.number: GetBundleArchiveInfo_0100
 * @tc.name: test can get the bundle archive info
 * @tc.desc: 1.system run normally
 *           2.get the bundle archive info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleArchiveInfo_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    BundleInfo testResult;
    bool listRet = hostImpl->GetBundleArchiveInfo(HAP_FILE_PATH, BundleFlag::GET_BUNDLE_DEFAULT, testResult);
    EXPECT_TRUE(listRet);
    CheckBundleArchiveInfo(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_SIZE_ZERO, testResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundleArchiveInfo_0200
 * @tc.name: test can not get the bundle archive info by empty hap file path
 * @tc.desc: 1.system run normally
 *           2.get the bundle archive info failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleArchiveInfo_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    BundleInfo testResult;
    bool listRet = hostImpl->GetBundleArchiveInfo("", BundleFlag::GET_BUNDLE_DEFAULT, testResult);
    EXPECT_FALSE(listRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundleArchiveInfo_0300
 * @tc.name: test can not get the bundle archive info by no exist hap file path
 * @tc.desc: 1.system run normally
 *           2.get the bundle archive info failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleArchiveInfo_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    BundleInfo testResult;
    bool listRet = hostImpl->GetBundleArchiveInfo(ERROR_HAP_FILE_PATH, BundleFlag::GET_BUNDLE_DEFAULT, testResult);
    EXPECT_FALSE(listRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundleArchiveInfo_0400
 * @tc.name: test can get the bundle archive info
 * @tc.desc: 1.system run normally
 *           2.get the bundle archive info successfully for GET_BUNDLE_WITH_ABILITIES
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleArchiveInfo_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    BundleInfo testResult;
    bool listRet = hostImpl->GetBundleArchiveInfo(HAP_FILE_PATH, BundleFlag::GET_BUNDLE_WITH_ABILITIES, testResult);
    EXPECT_TRUE(listRet);
    CheckBundleArchiveInfo(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_SIZE_ONE, testResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetHapModuleInfo_0100
 * @tc.name: test can get the hap module info
 * @tc.desc: 1.system run normally
 *           2.get the hap module info successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetHapModuleInfo_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    AbilityInfo abilityInfo;
    abilityInfo.bundleName = BUNDLE_NAME_TEST;
    abilityInfo.package = PACKAGE_NAME;

    HapModuleInfo hapModuleInfo;
    bool ret = GetBundleDataMgr()->GetHapModuleInfo(abilityInfo, hapModuleInfo);
    EXPECT_EQ(true, ret);
    CheckModuleInfo(hapModuleInfo);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetHapModuleInfo_0200
 * @tc.name: test can not get the hap module info by no exist bundleName
 * @tc.desc: 1.system run normally
 *           2.get the hap module info failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetHapModuleInfo_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    AbilityInfo abilityInfo;
    abilityInfo.bundleName = BUNDLE_NAME_DEMO;
    abilityInfo.package = PACKAGE_NAME;

    HapModuleInfo hapModuleInfo;
    bool ret = GetBundleDataMgr()->GetHapModuleInfo(abilityInfo, hapModuleInfo);
    EXPECT_EQ(false, ret);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetHapModuleInfo_0300
 * @tc.name: test can not get the hap module info by no exist package
 * @tc.desc: 1.system run normally
 *           2.get the hap module info failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetHapModuleInfo_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    AbilityInfo abilityInfo;
    abilityInfo.bundleName = BUNDLE_NAME_TEST;
    abilityInfo.package = BUNDLE_NAME_DEMO;

    HapModuleInfo hapModuleInfo;
    bool ret = GetBundleDataMgr()->GetHapModuleInfo(abilityInfo, hapModuleInfo);
    EXPECT_EQ(false, ret);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CheckApplicationEnabled_0100
 * @tc.name: test can check bundle status is enable by no setting
 * @tc.desc: 1.system run normally
 *           2.check the bundle status successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckApplicationEnabled_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    bool testRet = GetBundleDataMgr()->IsApplicationEnabled(BUNDLE_NAME_TEST);
    EXPECT_TRUE(testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CheckApplicationEnabled_0200
 * @tc.name: test can check bundle status is enable by setting
 * @tc.desc: 1.system run normally
 *           2.check the bundle status successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckApplicationEnabled_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    bool testRet = GetBundleDataMgr()->SetApplicationEnabled(BUNDLE_NAME_TEST, true);
    EXPECT_TRUE(testRet);
    bool testRet1 = GetBundleDataMgr()->IsApplicationEnabled(BUNDLE_NAME_TEST);
    EXPECT_TRUE(testRet1);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CheckApplicationEnabled_0300
 * @tc.name: test can check bundle status is disable by setting
 * @tc.desc: 1.system run normally
 *           2.check the bundle status successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckApplicationEnabled_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    bool testRet = GetBundleDataMgr()->SetApplicationEnabled(BUNDLE_NAME_TEST, false);
    EXPECT_TRUE(testRet);
    bool testRet1 = GetBundleDataMgr()->IsApplicationEnabled(BUNDLE_NAME_TEST);
    EXPECT_FALSE(testRet1);

    bool testRet2 = GetBundleDataMgr()->SetApplicationEnabled(BUNDLE_NAME_TEST, true);
    EXPECT_TRUE(testRet2);
    bool testRet3 = GetBundleDataMgr()->IsApplicationEnabled(BUNDLE_NAME_TEST);
    EXPECT_TRUE(testRet3);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CheckApplicationEnabled_0400
 * @tc.name: test can check bundle status is disable by no install
 * @tc.desc: 1.system run normally
 *           2.check the bundle status failed
 */
HWTEST_F(BmsBundleKitServiceTest, CheckApplicationEnabled_0400, Function | SmallTest | Level1)
{
    bool testRet = GetBundleDataMgr()->IsApplicationEnabled(BUNDLE_NAME_TEST);
    EXPECT_FALSE(testRet);
}

/**
 * @tc.number: CheckApplicationEnabled_0500
 * @tc.name: test can check bundle status is able by empty bundle name
 * @tc.desc: 1.system run normally
 *           2.check the bundle status successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckApplicationEnabled_0500, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    bool testRet = GetBundleDataMgr()->SetApplicationEnabled("", true);
    EXPECT_FALSE(testRet);
    bool testRet1 = GetBundleDataMgr()->IsApplicationEnabled(BUNDLE_NAME_TEST);
    EXPECT_TRUE(testRet1);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CheckApplicationEnabled_0600
 * @tc.name: test can check bundle status is disable by empty bundle name
 * @tc.desc: 1.system run normally
 *           2.check the bundle status failed
 */
HWTEST_F(BmsBundleKitServiceTest, CheckApplicationEnabled_0600, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    bool testRet = GetBundleDataMgr()->SetApplicationEnabled(BUNDLE_NAME_TEST, true);
    EXPECT_TRUE(testRet);
    bool testRet1 = GetBundleDataMgr()->IsApplicationEnabled("");
    EXPECT_FALSE(testRet1);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundleInfosByMetaData_0100
 * @tc.name: test can get the bundle infos by metadata
 * @tc.desc: 1.system run normally
 *           2.get bundle infos successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfosByMetaData_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<BundleInfo> bundleInfos;
    bool testRet = GetBundleDataMgr()->GetBundleInfosByMetaData(META_DATA, bundleInfos);
    EXPECT_EQ(true, testRet);
    CheckInstalledBundleInfos(ABILITY_SIZE_ONE, bundleInfos);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetBundleInfosByMetaData_0200
 * @tc.name: test can not get the bundle infos by empty metadata
 * @tc.desc: 1.system run normally
 *           2.get bundle infos failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfosByMetaData_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<BundleInfo> bundleInfos;
    bool testRet = GetBundleDataMgr()->GetBundleInfosByMetaData("", bundleInfos);
    EXPECT_EQ(false, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetBundleInfosByMetaData_0300
 * @tc.name: test can not get the bundle infos by no exist metadata
 * @tc.desc: 1.system run normally
 *           2.get bundle infos failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundleInfosByMetaData_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<BundleInfo> bundleInfos;
    bool testRet = GetBundleDataMgr()->GetBundleInfosByMetaData(ERROR_META_DATA, bundleInfos);
    EXPECT_EQ(false, testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: CleanBundleDataFiles_0100
 * @tc.name: test can clean the bundle data files by bundle name
 * @tc.desc: 1.system run normally
 *           2.clean the bundle data files successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CleanBundleDataFiles_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    CreateFileDir();

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    bool testRet = hostImpl->CleanBundleDataFiles(BUNDLE_NAME_TEST);
    EXPECT_TRUE(testRet);
    CheckFileNonExist();

    CleanFileDir();
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CleanBundleDataFiles_0200
 * @tc.name: test can clean the bundle data files by empty bundle name
 * @tc.desc: 1.system run normally
 *           2.clean the bundle data files failed
 */
HWTEST_F(BmsBundleKitServiceTest, CleanBundleDataFiles_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    CreateFileDir();

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    bool testRet = hostImpl->CleanBundleDataFiles("");
    EXPECT_FALSE(testRet);
    CheckFileExist();

    CleanFileDir();
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CleanBundleDataFiles_0300
 * @tc.name: test can clean the bundle data files by no exist bundle name
 * @tc.desc: 1.system run normally
 *           2.clean the bundle data files failed
 */
HWTEST_F(BmsBundleKitServiceTest, CleanBundleDataFiles_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    CreateFileDir();

    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    bool testRet = hostImpl->CleanBundleDataFiles(BUNDLE_NAME_DEMO);
    EXPECT_FALSE(testRet);
    CheckFileExist();

    CleanFileDir();
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CleanCache_0100
 * @tc.name: test can clean the cache files by bundle name
 * @tc.desc: 1.system run normally
 *           2.clean the cache files successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CleanCache_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    CreateFileDir();

    sptr<MockCleanCache> cleanCache = new (std::nothrow) MockCleanCache();
    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    bool result = hostImpl->CleanBundleCacheFiles(BUNDLE_NAME_TEST, cleanCache);
    EXPECT_TRUE(result);
    bool callbackResult = cleanCache->GetResultCode();
    CheckCacheNonExist();
    EXPECT_TRUE(callbackResult);

    CleanFileDir();
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CleanCache_0200
 * @tc.name: test can clean the cache files by empty bundle name
 * @tc.desc: 1.system run normally
 *           2.clean the cache files failed
 */
HWTEST_F(BmsBundleKitServiceTest, CleanCache_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    CreateFileDir();

    sptr<MockCleanCache> cleanCache = new (std::nothrow) MockCleanCache();
    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    bool result = hostImpl->CleanBundleCacheFiles("", cleanCache);
    EXPECT_FALSE(result);
    CheckCacheExist();

    CleanFileDir();
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CleanCache_0300
 * @tc.name: test can clean the cache files by no exist bundle name
 * @tc.desc: 1.system run normally
 *           2.clean the cache files failed
 */
HWTEST_F(BmsBundleKitServiceTest, CleanCache_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    CreateFileDir();

    sptr<MockCleanCache> cleanCache = new (std::nothrow) MockCleanCache();
    auto hostImpl = std::make_unique<BundleMgrHostImpl>();
    bool result = hostImpl->CleanBundleCacheFiles(BUNDLE_NAME_DEMO, cleanCache);
    EXPECT_FALSE(result);
    CheckCacheExist();

    CleanFileDir();
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: RegisterBundleStatus_0100
 * @tc.name: test can register the bundle status by bundle name
 * @tc.desc: 1.system run normally
 *           2.bundle status callback successfully
 */
HWTEST_F(BmsBundleKitServiceTest, RegisterBundleStatus_0100, Function | SmallTest | Level1)
{
    sptr<MockBundleStatus> bundleStatusCallback = new (std::nothrow) MockBundleStatus();
    bundleStatusCallback->SetBundleName(HAP_FILE_PATH);
    bool result = GetBundleDataMgr()->RegisterBundleStatusCallback(bundleStatusCallback);
    EXPECT_TRUE(result);

    bool resultNotify = GetBundleDataMgr()->NotifyBundleStatus(
        HAP_FILE_PATH, HAP_FILE_PATH, ABILITY_NAME_DEMO, ERR_OK, NotifyType::INSTALL);
    EXPECT_TRUE(resultNotify);

    int32_t callbackResult = bundleStatusCallback->GetResultCode();
    EXPECT_EQ(callbackResult, ERR_OK);
}

/**
 * @tc.number: RegisterBundleStatus_0200
 * @tc.name: test can register the bundle status by bundle name
 * @tc.desc: 1.system run normally
 *           2.bundle status callback failed by empty bundle name
 */
HWTEST_F(BmsBundleKitServiceTest, RegisterBundleStatus_0200, Function | SmallTest | Level1)
{
    sptr<MockBundleStatus> bundleStatusCallback = new (std::nothrow) MockBundleStatus();
    bundleStatusCallback->SetBundleName(HAP_FILE_PATH);
    bool result = GetBundleDataMgr()->RegisterBundleStatusCallback(bundleStatusCallback);
    EXPECT_TRUE(result);

    bool resultNotify =
        GetBundleDataMgr()->NotifyBundleStatus("", HAP_FILE_PATH, ABILITY_NAME_DEMO, ERR_OK, NotifyType::INSTALL);
    EXPECT_TRUE(resultNotify);

    int32_t callbackResult = bundleStatusCallback->GetResultCode();
    EXPECT_EQ(callbackResult, ERR_TIMED_OUT);
}

/**
 * @tc.number: RegisterBundleStatus_0300
 * @tc.name: test can register the bundle status by bundle name
 * @tc.desc: 1.system run normally
 *           2.bundle status callback failed by no exist bundle name
 */
HWTEST_F(BmsBundleKitServiceTest, RegisterBundleStatus_0300, Function | SmallTest | Level1)
{
    sptr<MockBundleStatus> bundleStatusCallback = new (std::nothrow) MockBundleStatus();
    bundleStatusCallback->SetBundleName(HAP_FILE_PATH);
    bool result = GetBundleDataMgr()->RegisterBundleStatusCallback(bundleStatusCallback);
    EXPECT_TRUE(result);

    bool resultNotify = GetBundleDataMgr()->NotifyBundleStatus(
        ERROR_HAP_FILE_PATH, HAP_FILE_PATH, ABILITY_NAME_DEMO, ERR_OK, NotifyType::INSTALL);
    EXPECT_TRUE(resultNotify);

    int32_t callbackResult = bundleStatusCallback->GetResultCode();
    EXPECT_EQ(callbackResult, ERR_TIMED_OUT);
}

/**
 * @tc.number: ClearBundleStatus_0100
 * @tc.name: test can clear the bundle status by bundle name
 * @tc.desc: 1.system run normally
 *           2.bundle status callback failed by cleared bundle name
 */
HWTEST_F(BmsBundleKitServiceTest, ClearBundleStatus_0100, Function | SmallTest | Level1)
{
    sptr<MockBundleStatus> bundleStatusCallback1 = new (std::nothrow) MockBundleStatus();
    bundleStatusCallback1->SetBundleName(HAP_FILE_PATH1);
    bool result1 = GetBundleDataMgr()->RegisterBundleStatusCallback(bundleStatusCallback1);
    EXPECT_TRUE(result1);

    bool result2 = GetBundleDataMgr()->ClearBundleStatusCallback(bundleStatusCallback1);
    EXPECT_TRUE(result2);

    sptr<MockBundleStatus> bundleStatusCallback = new (std::nothrow) MockBundleStatus();
    bundleStatusCallback->SetBundleName(HAP_FILE_PATH);
    bool result = GetBundleDataMgr()->RegisterBundleStatusCallback(bundleStatusCallback);
    EXPECT_TRUE(result);

    bool resultNotify = GetBundleDataMgr()->NotifyBundleStatus(
        HAP_FILE_PATH, HAP_FILE_PATH, ABILITY_NAME_DEMO, ERR_OK, NotifyType::INSTALL);
    EXPECT_TRUE(resultNotify);

    int32_t callbackResult = bundleStatusCallback->GetResultCode();
    EXPECT_EQ(callbackResult, ERR_OK);

    bool resultNotify1 = GetBundleDataMgr()->NotifyBundleStatus(
        HAP_FILE_PATH1, HAP_FILE_PATH, ABILITY_NAME_DEMO, ERR_OK, NotifyType::INSTALL);
    EXPECT_TRUE(resultNotify1);

    int32_t callbackResult1 = bundleStatusCallback1->GetResultCode();
    EXPECT_EQ(callbackResult1, ERR_TIMED_OUT);
}

/**
 * @tc.number: UnregisterBundleStatus_0100
 * @tc.name: test can unregister the bundle status by bundle name
 * @tc.desc: 1.system run normally
 *           2.bundle status callback failed by unregister bundle name
 */
HWTEST_F(BmsBundleKitServiceTest, UnregisterBundleStatus_0100, Function | SmallTest | Level1)
{
    sptr<MockBundleStatus> bundleStatusCallback = new (std::nothrow) MockBundleStatus();
    bundleStatusCallback->SetBundleName(HAP_FILE_PATH);
    bool result = GetBundleDataMgr()->RegisterBundleStatusCallback(bundleStatusCallback);
    EXPECT_TRUE(result);

    sptr<MockBundleStatus> bundleStatusCallback1 = new (std::nothrow) MockBundleStatus();
    bundleStatusCallback1->SetBundleName(HAP_FILE_PATH1);
    bool result1 = GetBundleDataMgr()->RegisterBundleStatusCallback(bundleStatusCallback1);
    EXPECT_TRUE(result1);

    bool result2 = GetBundleDataMgr()->UnregisterBundleStatusCallback();
    EXPECT_TRUE(result2);

    bool resultNotify = GetBundleDataMgr()->NotifyBundleStatus(
        HAP_FILE_PATH, HAP_FILE_PATH, ABILITY_NAME_DEMO, ERR_OK, NotifyType::INSTALL);
    EXPECT_TRUE(resultNotify);

    int32_t callbackResult = bundleStatusCallback->GetResultCode();
    EXPECT_EQ(callbackResult, ERR_TIMED_OUT);

    bool resultNotify1 = GetBundleDataMgr()->NotifyBundleStatus(
        HAP_FILE_PATH1, HAP_FILE_PATH, ABILITY_NAME_DEMO, ERR_OK, NotifyType::INSTALL);
    EXPECT_TRUE(resultNotify1);

    int32_t callbackResult1 = bundleStatusCallback1->GetResultCode();
    EXPECT_EQ(callbackResult1, ERR_TIMED_OUT);
}

/**
 * @tc.number: GetBundlesForUid_0100
 * @tc.name: test can get the bundle names with bundle installed
 * @tc.desc: 1.system run normally
 *           2.get installed bundle names successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundlesForUid_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<std::string> testResult;
    bool testRet = GetBundleDataMgr()->GetBundlesForUid(TEST_UID, testResult);
    EXPECT_TRUE(testRet);
    EXPECT_EQ(BUNDLE_NAMES_SIZE_ONE, testResult.size());
    EXPECT_EQ(BUNDLE_NAME_TEST, testResult[0]);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundlesForUid_0200
 * @tc.name: test can not get not installed bundle names
 * @tc.desc: 1.system run normally
 *           2.get installed bundle names by uid failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundlesForUid_0200, Function | SmallTest | Level1)
{
    std::vector<std::string> testResult;
    bool testRet = GetBundleDataMgr()->GetBundlesForUid(TEST_UID, testResult);
    EXPECT_FALSE(testRet);
    EXPECT_EQ(BUNDLE_NAMES_SIZE_ZERO, testResult.size());
}

/**
 * @tc.number: GetBundlesForUid_0300
 * @tc.name: test can not get installed bundle names by incorrect uid
 * @tc.desc: 1.system run normally
 *           2.get installed bundle names by uid failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundlesForUid_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::vector<std::string> testResult;
    bool testRet = GetBundleDataMgr()->GetBundlesForUid(DEMO_UID, testResult);
    EXPECT_FALSE(testRet);
    EXPECT_EQ(BUNDLE_NAMES_SIZE_ZERO, testResult.size());

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetBundlesForUid_0400
 * @tc.name: test can not get installed bundle names by invalid uid
 * @tc.desc: 1.system run normally
 *           2.get installed bundle names by uid failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetBundlesForUid_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);

    std::vector<std::string> testResult;
    bool testRet = GetBundleDataMgr()->GetBundlesForUid(INVALID_UID, testResult);
    EXPECT_FALSE(testRet);
    EXPECT_EQ(BUNDLE_NAMES_SIZE_ZERO, testResult.size());

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetNameForUid_0100
 * @tc.name: test can get the uid name with bundle installed
 * @tc.desc: 1.system run normally
 *           2.get installed uid name successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetNameForUid_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string testResult;
    bool testRet = GetBundleDataMgr()->GetNameForUid(TEST_UID, testResult);
    EXPECT_TRUE(testRet);
    EXPECT_EQ(BUNDLE_NAME_TEST, testResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetNameForUid_0200
 * @tc.name: test can not get not installed uid name
 * @tc.desc: 1.system run normally
 *           2.get installed uid name by uid failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetNameForUid_0200, Function | SmallTest | Level1)
{
    std::string testResult;
    bool testRet = GetBundleDataMgr()->GetNameForUid(TEST_UID, testResult);
    EXPECT_FALSE(testRet);
    EXPECT_NE(BUNDLE_NAME_TEST, testResult);
}

/**
 * @tc.number: GetNameForUid_0300
 * @tc.name: test can not get installed uid name by incorrect uid
 * @tc.desc: 1.system run normally
 *           2.get installed uid name by uid failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetNameForUid_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string testResult;
    bool testRet = GetBundleDataMgr()->GetNameForUid(DEMO_UID, testResult);
    EXPECT_FALSE(testRet);
    EXPECT_NE(BUNDLE_NAME_TEST, testResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetNameForUid_0400
 * @tc.name: test can not get installed uid name by invalid uid
 * @tc.desc: 1.system run normally
 *           2.get installed uid name by uid failed
 */
HWTEST_F(BmsBundleKitServiceTest, GetNameForUid_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);

    std::string testResult;
    bool testRet = GetBundleDataMgr()->GetNameForUid(INVALID_UID, testResult);
    EXPECT_FALSE(testRet);
    EXPECT_NE(BUNDLE_NAME_TEST, testResult);
    EXPECT_NE(BUNDLE_NAME_DEMO, testResult);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: CheckAbilityEnabled_0100
 * @tc.name: test can check ability status is enable by no setting
 * @tc.desc: 1.system run normally
 *           2.check the ability status successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckAbilityEnabled_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    AbilityInfo abilityInfo = MockAbilityInfo(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    bool testRet = GetBundleDataMgr()->IsAbilityEnabled(abilityInfo);
    EXPECT_TRUE(testRet);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CheckAbilityEnabled_0200
 * @tc.name: test can check ability status is enable by setting
 * @tc.desc: 1.system run normally
 *           2.check the ability status successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckAbilityEnabled_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    AbilityInfo abilityInfo = MockAbilityInfo(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    bool testRet = GetBundleDataMgr()->SetAbilityEnabled(abilityInfo, true);
    EXPECT_TRUE(testRet);
    bool testRet1 = GetBundleDataMgr()->IsAbilityEnabled(abilityInfo);
    EXPECT_TRUE(testRet1);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CheckAbilityEnabled_0300
 * @tc.name: test can check ability status is disable by setting and can be enabled again
 * @tc.desc: 1.system run normally
 *           2.check the ability status successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckAbilityEnabled_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    AbilityInfo abilityInfo = MockAbilityInfo(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    bool testRet = GetBundleDataMgr()->SetAbilityEnabled(abilityInfo, false);
    EXPECT_TRUE(testRet);
    bool testRet1 = GetBundleDataMgr()->IsAbilityEnabled(abilityInfo);
    EXPECT_FALSE(testRet1);
    bool testRet2 = GetBundleDataMgr()->SetAbilityEnabled(abilityInfo, true);
    EXPECT_TRUE(testRet2);
    bool testRet3 = GetBundleDataMgr()->IsAbilityEnabled(abilityInfo);
    EXPECT_TRUE(testRet3);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CheckAbilityEnabled_0400
 * @tc.name: test can check ability status is disable by no install
 * @tc.desc: 1.system run normally
 *           2.check the ability status failed
 */
HWTEST_F(BmsBundleKitServiceTest, CheckAbilityEnabled_0400, Function | SmallTest | Level1)
{
    AbilityInfo abilityInfo = MockAbilityInfo(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    bool testRet = GetBundleDataMgr()->IsAbilityEnabled(abilityInfo);
    EXPECT_FALSE(testRet);
}

/**
 * @tc.number: CheckAbilityEnabled_0500
 * @tc.name: test can check ability status is able by empty AbilityInfo
 * @tc.desc: 1.system run normally
 *           2.check the ability status successfully
 */
HWTEST_F(BmsBundleKitServiceTest, CheckAbilityEnabled_0500, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    AbilityInfo abilityInfoEmpty;
    bool testRet = GetBundleDataMgr()->SetAbilityEnabled(abilityInfoEmpty, false);
    EXPECT_FALSE(testRet);
    AbilityInfo abilityInfo = MockAbilityInfo(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    bool testRet1 = GetBundleDataMgr()->IsAbilityEnabled(abilityInfo);
    EXPECT_TRUE(testRet1);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: CheckAbilityEnabled_0600
 * @tc.name: test can check ability status is disable by empty AbilityInfo
 * @tc.desc: 1.system run normally
 *           2.check the ability status failed
 */
HWTEST_F(BmsBundleKitServiceTest, CheckAbilityEnabled_0600, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    AbilityInfo abilityInfo = MockAbilityInfo(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    bool testRet = GetBundleDataMgr()->SetAbilityEnabled(abilityInfo, true);
    EXPECT_TRUE(testRet);
    AbilityInfo abilityInfoEmpty;
    bool testRet1 = GetBundleDataMgr()->IsAbilityEnabled(abilityInfoEmpty);
    EXPECT_FALSE(testRet1);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetAbilityIcon_0100
 * @tc.name: test can get the ability's icon by bundleName and abilityName
 * @tc.desc: 1.system run normally
 *           2.get ability icon successfully
 */
HWTEST_F(BmsBundleKitServiceTest, GetAbilityIcon_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string icon = GetBundleDataMgr()->GetAbilityIcon(BUNDLE_NAME_TEST, ABILITY_NAME_TEST);
    EXPECT_EQ(ICON_PATH, icon);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetAbilityIcon_0200
 * @tc.name: test can not get the ability's icon if ability doesn't install
 * @tc.desc: 1.system run normally
 *           2.get empty ability icon
 */
HWTEST_F(BmsBundleKitServiceTest, GetAbilityIcon_0200, Function | SmallTest | Level1)
{
    std::string icon = GetBundleDataMgr()->GetAbilityIcon(BUNDLE_NAME_TEST, ABILITY_NAME_TEST);
    EXPECT_EQ(EMPTY_STRING, icon);
}

/**
 * @tc.number: GetAbilityIcon_0300
 * @tc.name: test can not get the ability's icon if ability doesn't exist
 * @tc.desc: 1.system run normally
 *           2.get empty ability icon
 */
HWTEST_F(BmsBundleKitServiceTest, GetAbilityIcon_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string icon = GetBundleDataMgr()->GetAbilityIcon(BUNDLE_NAME_DEMO, ABILITY_NAME_TEST);
    EXPECT_EQ(EMPTY_STRING, icon);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetAbilityIcon_0400
 * @tc.name: test can not get the ability's icon if ability doesn't exist
 * @tc.desc: 1.system run normally
 *           2.get empty ability icon
 */
HWTEST_F(BmsBundleKitServiceTest, GetAbilityIcon_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    std::string icon = GetBundleDataMgr()->GetAbilityIcon(BUNDLE_NAME_TEST, ABILITY_NAME_DEMO);
    EXPECT_EQ(EMPTY_STRING, icon);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}
/**
 * @tc.number: GetFormInfoByModule_0100
 * @tc.name: test can  get the formInfo
 * @tc.desc: 1.system run normally
 *           2.get formInfo by moduleName successful
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByModule_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfos;
    auto result = GetBundleDataMgr()->GetFormsInfoByModule(BUNDLE_NAME_TEST, MODULE_NAME_TEST, formInfos);
    EXPECT_EQ(result, true);
    EXPECT_FALSE(formInfos.empty());
    CheckFormInfoTest(formInfos);
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetFormInfoByModule_0200
 * @tc.name: test can get the formInfo
 * @tc.desc: 1.system run normally
 *           2.get forms info in different bundle
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByModule_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfo1;
    auto result = GetBundleDataMgr()->GetFormsInfoByModule(BUNDLE_NAME_TEST, MODULE_NAME_TEST, formInfo1);
    EXPECT_EQ(result, true);
    EXPECT_FALSE(formInfo1.empty());
    CheckFormInfoTest(formInfo1);

    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfo2;
    auto result1 = GetBundleDataMgr()->GetFormsInfoByModule(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, formInfo2);
    EXPECT_EQ(result1, true);
    EXPECT_FALSE(formInfo2.empty());
    CheckFormInfoDemo(formInfo2);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetFormInfoByModule_0400
 * @tc.name: test can not get the formInfo
 * @tc.desc:1.system run normally
 *          2.get form info with error moduleName
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByModule_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfos;
    GetBundleDataMgr()->GetFormsInfoByModule(BUNDLE_NAME_TEST, MODULE_NAME_DEMO, formInfos);
    EXPECT_TRUE(formInfos.empty());
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetFormInfoByModule_0400
 * @tc.name: test can not get the formInfo
 * @tc.desc: 1.system run normally
 *           2.get form info with empty moduleName
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByModule_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_DEMO);
    std::vector<FormInfo> formInfos;
    GetBundleDataMgr()->GetFormsInfoByModule(BUNDLE_NAME_TEST, EMPTY_STRING, formInfos);
    EXPECT_TRUE(formInfos.empty());
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetFormInfoByModule_0500
 * @tc.name: test can not get the formInfo
 * @tc.desc: 1.system run normally
 *           2.get form info with non bundle
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByModule_0500, Function | SmallTest | Level1)
{
    std::vector<FormInfo> formInfos;
    GetBundleDataMgr()->GetFormsInfoByModule(BUNDLE_NAME_TEST, EMPTY_STRING, formInfos);
    EXPECT_TRUE(formInfos.empty());
}

/**
 * @tc.number: GetFormInfoByApp_0100
 * @tc.name: test can get the formInfo by bundlename
 * @tc.desc: 1.system run normally
 *           2.get form info by bundleName
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByApp_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfos;
    auto result = GetBundleDataMgr()->GetFormsInfoByApp(BUNDLE_NAME_TEST, formInfos);
    EXPECT_EQ(result, true);
    EXPECT_FALSE(formInfos.empty());
    CheckFormInfoTest(formInfos);
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetFormInfoByApp_0200
 * @tc.name: test can get the formInfo in different app by bundlename
 * @tc.desc: 1.system run normally
 *           2.get form info by different bundleName
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByApp_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfo1;
    auto result = GetBundleDataMgr()->GetFormsInfoByApp(BUNDLE_NAME_TEST, formInfo1);
    EXPECT_EQ(result, true);
    EXPECT_FALSE(formInfo1.empty());
    CheckFormInfoTest(formInfo1);

    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfo2;
    auto result1 = GetBundleDataMgr()->GetFormsInfoByApp(BUNDLE_NAME_DEMO, formInfo2);
    EXPECT_EQ(result1, true);
    EXPECT_FALSE(formInfo2.empty());
    CheckFormInfoDemo(formInfo2);

    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetFormInfoByApp_0300
 * @tc.name: test can't get the formInfo in app by error app name
 * @tc.desc: 1.system run normally
 *           2.get form info by error bundleName
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByApp_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfo1;
    GetBundleDataMgr()->GetFormsInfoByApp(BUNDLE_NAME_DEMO, formInfo1);
    EXPECT_TRUE(formInfo1.empty());
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetFormInfoByApp_0400
 * @tc.name: test can't get the formInfo in app by empty app name
 * @tc.desc: 1.system run normally
 *           2.get form info by empty bundleName
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByApp_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfo1;
    GetBundleDataMgr()->GetFormsInfoByApp(EMPTY_STRING, formInfo1);
    EXPECT_TRUE(formInfo1.empty());
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetFormInfoByApp_0500
 * @tc.name: test can't get the formInfo have no bundle
 * @tc.desc: 1.system run normally
 *           2.get form info with non bundle
 */
HWTEST_F(BmsBundleKitServiceTest, GetFormInfoByApp_0500, Function | SmallTest | Level1)
{
    std::vector<FormInfo> formInfo1;
    GetBundleDataMgr()->GetFormsInfoByApp(BUNDLE_NAME_DEMO, formInfo1);
    EXPECT_TRUE(formInfo1.empty());
}

/**
 * @tc.number: GetAllFormInfo_0100
 * @tc.name: test can get all the formInfo
 * @tc.desc: 1.system run normally
 *           2.get forms by all the bundle
 */
HWTEST_F(BmsBundleKitServiceTest, GetAllFormInfo_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfos;
    auto result = GetBundleDataMgr()->GetAllFormsInfo(formInfos);
    EXPECT_EQ(result, true);
    EXPECT_FALSE(formInfos.empty());
    CheckFormInfoTest(formInfos);
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetAllFormInfo_0100
 * @tc.name: test can get all the formInfo
 * @tc.desc: 1.system run normally
 *           2.get forms by all the bundle
 */
HWTEST_F(BmsBundleKitServiceTest, GetAllFormInfo_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_TEST);
    std::vector<FormInfo> formInfos;
    auto result = GetBundleDataMgr()->GetAllFormsInfo(formInfos);
    EXPECT_FALSE(formInfos.empty());
    EXPECT_EQ(result, true);
    for (const auto &info : formInfos) {
        if (info.bundleName != BUNDLE_NAME_TEST) {
            std::vector<FormInfo> formInfo1;
            formInfo1.emplace_back(info);
            CheckFormInfoDemo(formInfo1);
        } else {
            std::vector<FormInfo> formInfo2;
            formInfo2.emplace_back(info);
            CheckFormInfoTest(formInfo2);
        }
    }
    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetAllFormInfo_0300
 * @tc.name: test can not get have no form
 * @tc.desc: 1.system run normally
 *           2.get forms by all the bundle
 */
HWTEST_F(BmsBundleKitServiceTest, GetAllFormInfo_0300, Function | SmallTest | Level1)
{
    std::vector<FormInfo> formInfos;
    GetBundleDataMgr()->GetAllFormsInfo(formInfos);
    EXPECT_TRUE(formInfos.empty());
}

/**
 * @tc.number: GetShortcutInfos_0100
 * @tc.name: test can get shortcutInfo by bundleName
 * @tc.desc: 1.can get shortcutInfo
 */
HWTEST_F(BmsBundleKitServiceTest, GetShortcutInfos_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<ShortcutInfo> shortcutInfos;
    auto result = GetBundleDataMgr()->GetShortcutInfos(BUNDLE_NAME_TEST, shortcutInfos);
    EXPECT_TRUE(result);
    CheckShortcutInfoTest(shortcutInfos);
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetShortcutInfos_0200
 * @tc.name: test have two bundle can get shortcutInfo by bundleName
 * @tc.desc: 1.can get shortcutInfo
 */
HWTEST_F(BmsBundleKitServiceTest, GetShortcutInfos_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_TEST);
    std::vector<ShortcutInfo> shortcutInfo1;
    auto result1 = GetBundleDataMgr()->GetShortcutInfos(BUNDLE_NAME_TEST, shortcutInfo1);
    EXPECT_TRUE(result1);
    CheckShortcutInfoTest(shortcutInfo1);
    std::vector<ShortcutInfo> shortcutInfo2;
    auto result2 = GetBundleDataMgr()->GetShortcutInfos(BUNDLE_NAME_DEMO, shortcutInfo2);
    EXPECT_TRUE(result2);
    CheckShortcutInfoDemo(shortcutInfo2);
    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: GetShortcutInfos_0300
 * @tc.name: test can't get the shortcutInfo in app by error app name
 * @tc.desc: 1.have not get shortcutinfo by appName
 */
HWTEST_F(BmsBundleKitServiceTest, GetShortcutInfos_0300, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<ShortcutInfo> shortcutInfos;
    GetBundleDataMgr()->GetShortcutInfos(BUNDLE_NAME_DEMO, shortcutInfos);
    EXPECT_TRUE(shortcutInfos.empty());
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetShortcutInfos_0400
 * @tc.name: test can't get the shortcutInfo in app by null app name
 * @tc.desc: 1.have not get shortcutinfo by null app name
 */
HWTEST_F(BmsBundleKitServiceTest, GetShortcutInfos_0400, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    std::vector<ShortcutInfo> shortcutInfos;
    GetBundleDataMgr()->GetShortcutInfos(EMPTY_STRING, shortcutInfos);
    EXPECT_TRUE(shortcutInfos.empty());
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: GetShortcutInfos_0500
 * @tc.name: test can't get the shortcutInfo have no bundle
 * @tc.desc: 1.have not get shortcutInfo by appName
 *           2.can't get shortcutInfo
 */
HWTEST_F(BmsBundleKitServiceTest, GetShortcutInfos_0500, Function | SmallTest | Level1)
{
    std::vector<ShortcutInfo> shortcutInfos;
    GetBundleDataMgr()->GetShortcutInfos(BUNDLE_NAME_TEST, shortcutInfos);
    EXPECT_TRUE(shortcutInfos.empty());
}

/**
 * @tc.number: Ability_0100
 * @tc.name: test can get the compatibleAbilityInfo by one bundle
 * @tc.desc: 1.can get compatibleAbilityInfo
 */
HWTEST_F(BmsBundleKitServiceTest, Ability_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    Want want;
    want.SetElementName(BUNDLE_NAME_TEST, ABILITY_NAME_TEST);
    AbilityInfo abilityInfo;
    bool testRet = GetBundleDataMgr()->QueryAbilityInfo(want, abilityInfo);
    EXPECT_TRUE(testRet);
    CompatibleAbilityInfo compatibleAbilityInfo;
    abilityInfo.ConvertToCompatiableAbilityInfo(compatibleAbilityInfo);
    CheckCompatibleAbilityInfo(BUNDLE_NAME_TEST, ABILITY_NAME_TEST, compatibleAbilityInfo);
    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: Ability_0200
 * @tc.name: test can get the compatibleAbilityInfo by two bundle
 * @tc.desc: 1.can get compatibleAbilityInfo
 */
HWTEST_F(BmsBundleKitServiceTest, Ability_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);
    Want want1;
    want1.SetElementName(BUNDLE_NAME_TEST, ABILITY_NAME_TEST);
    AbilityInfo abilityInfo1;
    bool testRet1 = GetBundleDataMgr()->QueryAbilityInfo(want1, abilityInfo1);
    EXPECT_TRUE(testRet1);
    CompatibleAbilityInfo compatibleAbilityInfo1;
    abilityInfo1.ConvertToCompatiableAbilityInfo(compatibleAbilityInfo1);
    CheckCompatibleAbilityInfo(BUNDLE_NAME_TEST, ABILITY_NAME_TEST, compatibleAbilityInfo1);
    Want want2;
    want2.SetElementName(BUNDLE_NAME_DEMO, ABILITY_NAME_DEMO);
    AbilityInfo abilityInfo2;
    bool testRet2 = GetBundleDataMgr()->QueryAbilityInfo(want2, abilityInfo2);
    EXPECT_TRUE(testRet2);
    CompatibleAbilityInfo compatibleAbilityInfo2;
    abilityInfo2.ConvertToCompatiableAbilityInfo(compatibleAbilityInfo2);
    CheckCompatibleAbilityInfo(BUNDLE_NAME_DEMO, ABILITY_NAME_DEMO, compatibleAbilityInfo2);
    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}

/**
 * @tc.number: Application_0100
 * @tc.name: test can get the compatibleApplicationInfo by one bundle
 * @tc.desc: 1.can get compatibleApplicationInfo
 */
HWTEST_F(BmsBundleKitServiceTest, Application_0100, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);

    ApplicationInfo testResult;
    bool testRet = GetBundleDataMgr()->GetApplicationInfo(
        BUNDLE_NAME_TEST, ApplicationFlag::GET_BASIC_APPLICATION_INFO, DEFAULT_USER_ID, testResult);
    EXPECT_TRUE(testRet);
    CompatibleApplicationInfo appInfo;
    testResult.ConvertToCompatibleApplicationInfo(appInfo);
    CheckCompatibleApplicationInfo(BUNDLE_NAME_TEST, PERMISSION_SIZE_ZERO, appInfo);

    MockUninstallBundle(BUNDLE_NAME_TEST);
}

/**
 * @tc.number: Application_0100
 * @tc.name: test can get the compatibleApplicationInfo by two bundle
 * @tc.desc: 1.can get compatibleApplicationInfo
 */
HWTEST_F(BmsBundleKitServiceTest, Application_0200, Function | SmallTest | Level1)
{
    MockInstallBundle(BUNDLE_NAME_TEST, MODULE_NAME_TEST, ABILITY_NAME_TEST);
    MockInstallBundle(BUNDLE_NAME_DEMO, MODULE_NAME_DEMO, ABILITY_NAME_DEMO);

    ApplicationInfo testResult1;
    bool testRet1 = GetBundleDataMgr()->GetApplicationInfo(
        BUNDLE_NAME_TEST, ApplicationFlag::GET_BASIC_APPLICATION_INFO, DEFAULT_USER_ID, testResult1);
    EXPECT_TRUE(testRet1);
    CompatibleApplicationInfo appInfo1;
    testResult1.ConvertToCompatibleApplicationInfo(appInfo1);
    CheckCompatibleApplicationInfo(BUNDLE_NAME_TEST, PERMISSION_SIZE_ZERO, appInfo1);

    ApplicationInfo testResult2;
    bool testRet2 = GetBundleDataMgr()->GetApplicationInfo(
        BUNDLE_NAME_DEMO, ApplicationFlag::GET_BASIC_APPLICATION_INFO, DEFAULT_USER_ID, testResult2);
    EXPECT_TRUE(testRet2);
    CompatibleApplicationInfo appInfo2;
    testResult2.ConvertToCompatibleApplicationInfo(appInfo2);
    CheckCompatibleApplicationInfo(BUNDLE_NAME_DEMO, PERMISSION_SIZE_ZERO, appInfo2);
    MockUninstallBundle(BUNDLE_NAME_TEST);
    MockUninstallBundle(BUNDLE_NAME_DEMO);
}
