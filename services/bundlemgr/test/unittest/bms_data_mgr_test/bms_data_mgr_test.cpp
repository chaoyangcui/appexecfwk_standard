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

#include "app_log_wrapper.h"
#include "json_constants.h"
#include "json_serializer.h"
#include "parcel.h"
#include "bundle_data_storage_interface.h"
#include "bundle_data_storage_database.h"
#include "bundle_data_mgr.h"

using namespace testing::ext;
using namespace OHOS::AppExecFwk;
using OHOS::Parcel;
using OHOS::AAFwk::Want;

namespace {

const std::string BUNDLE_NAME = "com.example.l3jsdemo";
const std::string APP_NAME = "com.example.l3jsdemo";
const std::string ABILITY_NAME = "com.example.l3jsdemo.MainAbility";
const std::string PACKAGE_NAME = "com.example.l3jsdemo";
const std::string DEVICE_ID = "PHONE-001";
const std::string LABEL = "hello";
const std::string DESCRIPTION = "mainEntry";
const std::string ICON_PATH = "/data/data/icon.png";
const std::string KIND = "test";
const AbilityType ABILITY_TYPE = AbilityType::PAGE;
const DisplayOrientation ORIENTATION = DisplayOrientation::PORTRAIT;
const LaunchMode LAUNCH_MODE = LaunchMode::SINGLETON;
const std::string CODE_PATH = "/data/accounts/account_/com.example.l3jsdemo";
const std::string RESOURCE_PATH = "/data/accounts/account_/com.example.l3jsdemo";
const std::string LIB_PATH = "/data/accounts/account_/com.example.l3jsdemo";
const bool VISIBLE = true;
const int32_t USERID = 0;

}  // namespace

class BmsDataMgrTest : public testing::Test {
public:
    BmsDataMgrTest();
    ~BmsDataMgrTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    const std::shared_ptr<BundleDataMgr> GetDataMgr() const;
    AbilityInfo GetDefaultAbilityInfo() const;

private:
    std::shared_ptr<BundleDataMgr> dataMgr_ = std::make_shared<BundleDataMgr>();
};

BmsDataMgrTest::BmsDataMgrTest()
{}

BmsDataMgrTest::~BmsDataMgrTest()
{}

void BmsDataMgrTest::SetUpTestCase()
{}

void BmsDataMgrTest::TearDownTestCase()
{}

void BmsDataMgrTest::SetUp()
{}

void BmsDataMgrTest::TearDown()
{
    dataMgr_->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

AbilityInfo BmsDataMgrTest::GetDefaultAbilityInfo() const
{
    AbilityInfo abilityInfo;
    abilityInfo.package = PACKAGE_NAME;
    abilityInfo.name = ABILITY_NAME;
    abilityInfo.bundleName = BUNDLE_NAME;
    abilityInfo.applicationName = APP_NAME;
    abilityInfo.deviceId = DEVICE_ID;
    abilityInfo.label = LABEL;
    abilityInfo.description = DESCRIPTION;
    abilityInfo.iconPath = ICON_PATH;
    abilityInfo.visible = VISIBLE;
    abilityInfo.kind = KIND;
    abilityInfo.type = ABILITY_TYPE;
    abilityInfo.orientation = ORIENTATION;
    abilityInfo.launchMode = LAUNCH_MODE;
    abilityInfo.codePath = CODE_PATH;
    abilityInfo.resourcePath = RESOURCE_PATH;
    abilityInfo.libPath = LIB_PATH;
    return abilityInfo;
}

const std::shared_ptr<BundleDataMgr> BmsDataMgrTest::GetDataMgr() const
{
    return dataMgr_;
}

/**
 * @tc.number: UpdateInstallState_0100
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. correct status transfer INSTALL_START->INSTALL_FAIL
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_0100, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
}

/**
 * @tc.number: UpdateInstallState_0200
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. correct status transfer INSTALL_START->INSTALL_SUCCESS->UPDATING_START->UPDATING_FAIL
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_0200, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
    bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_FAIL);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);
}

/**
 * @tc.number: UpdateInstallState_0300
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. correct status transfer INSTALL_START->INSTALL_SUCCESS->UPDATING_START->UPDATING_SUCCESS
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_0300, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
    bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: UpdateInstallState_0400
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. correct status transfer INSTALL_START->INSTALL_SUCCESS->UNINSTALL_START->UNINSTALL_SUCCESS
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_0400, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);
}

/**
 * @tc.number: UpdateInstallState_0500
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. correct status transfer INSTALL_START->INSTALL_SUCCESS->UNINSTALL_START->UNINSTALL_FAIL
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_0500, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_FAIL);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);
}

/**
 * @tc.number: UpdateInstallState_0600
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_START
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_0600, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    EXPECT_TRUE(ret1);
    EXPECT_FALSE(ret2);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
}

/**
 * @tc.number: UpdateInstallState_0700
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->UNINSTALL_START
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_0700, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    EXPECT_TRUE(ret1);
    EXPECT_FALSE(ret2);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
}

/**
 * @tc.number: UpdateInstallState_0800
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->UNINSTALL_SUCCESS
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_0800, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
    EXPECT_TRUE(ret1);
    EXPECT_FALSE(ret2);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
}

/**
 * @tc.number: UpdateInstallState_0900
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->UNINSTALL_FAIL
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_0900, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_FAIL);
    EXPECT_TRUE(ret1);
    EXPECT_FALSE(ret2);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
}

/**
 * @tc.number: UpdateInstallState_1000
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->UPDATING_STAR
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1000, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
    EXPECT_TRUE(ret1);
    EXPECT_FALSE(ret2);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
}

/**
 * @tc.number: UpdateInstallState_1100
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->UPDATING_SUCCESS
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1100, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
    EXPECT_TRUE(ret1);
    EXPECT_FALSE(ret2);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
}

/**
 * @tc.number: UpdateInstallState_1200
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->UPDATING_FAIL
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1200, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_FAIL);
    EXPECT_TRUE(ret1);
    EXPECT_FALSE(ret2);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
}

/**
 * @tc.number: UpdateInstallState_1300
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->INSTALL_SUCCESS
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1300, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_FALSE(ret3);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: UpdateInstallState_1400
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->INSTALL_START
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1400, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_FALSE(ret3);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: UpdateInstallState_1500
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->INSTALL_FAIL
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1500, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_FALSE(ret3);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: UpdateInstallState_1600
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->UNINSTALL_SUCCESS
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1600, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_FALSE(ret3);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: UpdateInstallState_1700
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->UNINSTALL_FAIL
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1700, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_FAIL);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_FALSE(ret3);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: UpdateInstallState_1800
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->UPDATING_FAIL
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1800, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_FAIL);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_FALSE(ret3);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: UpdateInstallState_1900
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->UPDATING_SUCCESS
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_1900, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_FALSE(ret3);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: UpdateInstallState_2000
 * @tc.name: UpdateInstallState
 * @tc.desc: 1. empty bundle name
 *           2. verify function return value
 */
HWTEST_F(BmsDataMgrTest, UpdateInstallState_2000, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState("", InstallState::INSTALL_START);
    EXPECT_FALSE(ret1);
}

/**
 * @tc.number: LoadDataFromPersistentStorage_0100
 * @tc.name: LoadDataFromPersistentStorage
 * @tc.desc: 1. NA
 *           2. load data from persistent storage
 */
HWTEST_F(BmsDataMgrTest, LoadDataFromPersistentStorage_0100, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->LoadDataFromPersistentStorage();
    EXPECT_FALSE(ret1);

    InnerBundleInfo info;
    BundleInfo bundleInfo;
    bundleInfo.name = BUNDLE_NAME;
    bundleInfo.applicationInfo.name = APP_NAME;
    ApplicationInfo applicationInfo;
    applicationInfo.name = BUNDLE_NAME;
    info.SetBaseBundleInfo(bundleInfo);
    info.SetBaseApplicationInfo(applicationInfo);

    std::shared_ptr<IBundleDataStorage> dataStorage_ = std::make_shared<BundleDataStorageDatabase>();
    dataStorage_->SaveStorageBundleInfo(Constants::CURRENT_DEVICE_ID, info);

    bool ret2 = dataMgr->LoadDataFromPersistentStorage();
    EXPECT_TRUE(ret2);
    dataStorage_->DeleteStorageBundleInfo(Constants::CURRENT_DEVICE_ID, info);
}

/**
 * @tc.number: AddBundleInfo_0100
 * @tc.name: AddBundleInfo
 * @tc.desc: 1. add info to the data manager
 *           2. query data then verify
 */
HWTEST_F(BmsDataMgrTest, AddBundleInfo_0100, Function | SmallTest | Level0)
{
    InnerBundleInfo info;
    BundleInfo bundleInfo;
    bundleInfo.name = BUNDLE_NAME;
    bundleInfo.applicationInfo.name = APP_NAME;
    ApplicationInfo applicationInfo;
    applicationInfo.name = BUNDLE_NAME;
    applicationInfo.deviceId = DEVICE_ID;
    info.SetBaseBundleInfo(bundleInfo);
    info.SetBaseApplicationInfo(applicationInfo);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    InnerBundleInfo info1;
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
    bool ret4 = dataMgr->GetInnerBundleInfo(BUNDLE_NAME, DEVICE_ID, info1);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);

    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: AddBundleInfo_0200
 * @tc.name: AddBundleInfo
 * @tc.desc: 1. add info to the data manager
 *           2. query data then verify
 */
HWTEST_F(BmsDataMgrTest, AddBundleInfo_0200, Function | SmallTest | Level0)
{
    InnerBundleInfo info1;
    BundleInfo bundleInfo1;
    bundleInfo1.name = BUNDLE_NAME;
    bundleInfo1.applicationInfo.name = APP_NAME;
    bundleInfo1.applicationInfo.bundleName = BUNDLE_NAME;
    ApplicationInfo applicationInfo1;
    applicationInfo1.name = BUNDLE_NAME;
    applicationInfo1.deviceId = DEVICE_ID;
    info1.SetBaseBundleInfo(bundleInfo1);
    info1.SetBaseApplicationInfo(applicationInfo1);

    InnerBundleInfo info2;
    BundleInfo bundleInfo2;
    bundleInfo2.name = BUNDLE_NAME;
    bundleInfo2.applicationInfo.name = APP_NAME;
    bundleInfo2.applicationInfo.bundleName = BUNDLE_NAME;
    ApplicationInfo applicationInfo2;
    applicationInfo2.name = BUNDLE_NAME;
    applicationInfo2.deviceId = DEVICE_ID;
    info2.SetBaseBundleInfo(bundleInfo2);
    info2.SetBaseApplicationInfo(applicationInfo2);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info1);
    bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
    bool ret5 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
    bool ret6 = dataMgr->UpdateInnerBundleInfo(BUNDLE_NAME, info2, info1);
    bool ret7 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);
    EXPECT_TRUE(ret5);
    EXPECT_TRUE(ret6);
    EXPECT_TRUE(ret7);

    ApplicationInfo appInfo;
    bool ret8 = dataMgr->GetApplicationInfo(BUNDLE_NAME, ApplicationFlag::GET_BASIC_APPLICATION_INFO, USERID, appInfo);
    EXPECT_TRUE(ret8);

    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: AddBundleInfo_0300
 * @tc.name: AddBundleInfo
 * @tc.desc: 1. scan dir not exist
 *           2. verify scan result file number is 0
 */
HWTEST_F(BmsDataMgrTest, AddBundleInfo_0300, Function | SmallTest | Level0)
{
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    InnerBundleInfo info;
    bool ret = dataMgr->AddInnerBundleInfo("", info);
    EXPECT_FALSE(ret);
}

/**
 * @tc.number: AddBundleInfo_0400
 * @tc.name: AddBundleInfo
 * @tc.desc: 1. add info to the data manager, then uninstall, then reinstall
 *           2. query data then verify
 */
HWTEST_F(BmsDataMgrTest, AddBundleInfo_0400, Function | SmallTest | Level0)
{
    InnerBundleInfo info;
    BundleInfo bundleInfo;
    bundleInfo.name = BUNDLE_NAME;
    bundleInfo.applicationInfo.name = APP_NAME;
    ApplicationInfo applicationInfo;
    applicationInfo.name = BUNDLE_NAME;
    applicationInfo.deviceId = DEVICE_ID;
    info.SetBaseBundleInfo(bundleInfo);
    info.SetBaseApplicationInfo(applicationInfo);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);

    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
    bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret5 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret6 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
    EXPECT_TRUE(ret4);
    EXPECT_TRUE(ret5);
    EXPECT_TRUE(ret6);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: AddBundleInfo_0500
 * @tc.name: AddBundleInfo
 * @tc.desc: 1. add module info to the data manager
 *           2. query data then verify
 */
HWTEST_F(BmsDataMgrTest, AddBundleInfo_0500, Function | SmallTest | Level0)
{
    InnerBundleInfo info1;
    BundleInfo bundleInfo1;
    bundleInfo1.name = BUNDLE_NAME;
    bundleInfo1.applicationInfo.name = APP_NAME;
    bundleInfo1.applicationInfo.bundleName = BUNDLE_NAME;
    ApplicationInfo applicationInfo1;
    applicationInfo1.name = BUNDLE_NAME;
    applicationInfo1.deviceId = DEVICE_ID;
    info1.SetBaseBundleInfo(bundleInfo1);
    info1.SetBaseApplicationInfo(applicationInfo1);

    InnerBundleInfo info2;
    BundleInfo bundleInfo2;
    bundleInfo2.name = BUNDLE_NAME;
    bundleInfo2.applicationInfo.name = APP_NAME;
    bundleInfo2.applicationInfo.bundleName = BUNDLE_NAME;
    ApplicationInfo applicationInfo2;
    applicationInfo2.name = BUNDLE_NAME;
    applicationInfo2.deviceId = DEVICE_ID;
    info2.SetBaseBundleInfo(bundleInfo2);
    info2.SetBaseApplicationInfo(applicationInfo2);

    InnerBundleInfo info3;
    InnerBundleInfo info4;
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info1);
    bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
    bool ret5 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
    bool ret6 = dataMgr->AddNewModuleInfo(BUNDLE_NAME, info2, info1);
    bool ret7 = dataMgr->GetInnerBundleInfo(BUNDLE_NAME, DEVICE_ID, info3);
    bool ret8 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret9 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
    bool ret10 = dataMgr->RemoveModuleInfo(BUNDLE_NAME, PACKAGE_NAME, info1);
    bool ret11 = dataMgr->GetInnerBundleInfo(BUNDLE_NAME, DEVICE_ID, info4);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);
    EXPECT_TRUE(ret5);
    EXPECT_TRUE(ret6);
    EXPECT_TRUE(ret7);
    EXPECT_TRUE(ret8);
    EXPECT_TRUE(ret9);
    EXPECT_TRUE(ret10);
    EXPECT_TRUE(ret11);

    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: GenerateUidAndGid_0100
 * @tc.name: GenerateUidAndGid
 * @tc.desc: 1. app type is system app
 *           2. generate uid and gid then verify
 */
HWTEST_F(BmsDataMgrTest, GenerateUidAndGid_0100, Function | SmallTest | Level0)
{
    InnerBundleInfo info;
    BundleInfo bundleInfo;
    bundleInfo.name = BUNDLE_NAME;
    bundleInfo.applicationInfo.name = APP_NAME;
    ApplicationInfo applicationInfo;
    applicationInfo.name = BUNDLE_NAME;
    applicationInfo.deviceId = DEVICE_ID;
    info.SetBaseBundleInfo(bundleInfo);
    info.SetBaseApplicationInfo(applicationInfo);
    info.SetAppType(Constants::AppType::SYSTEM_APP);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
    bool ret4 = dataMgr->GenerateUidAndGid(info);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);

    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: GenerateUidAndGid_0200
 * @tc.name: GenerateUidAndGid
 * @tc.desc: 1. app type is third party app
 *           2. generate uid and gid then verify
 */
HWTEST_F(BmsDataMgrTest, GenerateUidAndGid_0200, Function | SmallTest | Level0)
{
    InnerBundleInfo info;
    BundleInfo bundleInfo;
    bundleInfo.name = BUNDLE_NAME;
    bundleInfo.applicationInfo.name = APP_NAME;
    ApplicationInfo applicationInfo;
    applicationInfo.name = BUNDLE_NAME;
    applicationInfo.deviceId = DEVICE_ID;
    info.SetBaseBundleInfo(bundleInfo);
    info.SetBaseApplicationInfo(applicationInfo);
    info.SetAppType(Constants::AppType::THIRD_SYSTEM_APP);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
    bool ret4 = dataMgr->GenerateUidAndGid(info);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);

    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: GenerateUidAndGid_0300
 * @tc.name: GenerateUidAndGid
 * @tc.desc: 1. app type is third party app
 *           2. generate uid and gid then verify
 */
HWTEST_F(BmsDataMgrTest, GenerateUidAndGid_0300, Function | SmallTest | Level0)
{
    InnerBundleInfo info;
    BundleInfo bundleInfo;
    bundleInfo.name = BUNDLE_NAME;
    bundleInfo.applicationInfo.name = APP_NAME;
    ApplicationInfo applicationInfo;
    applicationInfo.name = BUNDLE_NAME;
    applicationInfo.deviceId = DEVICE_ID;
    info.SetBaseBundleInfo(bundleInfo);
    info.SetBaseApplicationInfo(applicationInfo);
    info.SetAppType(Constants::AppType::THIRD_PARTY_APP);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
    bool ret4 = dataMgr->GenerateUidAndGid(info);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);

    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: QueryAbilityInfo_0100
 * @tc.name: QueryAbilityInfo
 * @tc.desc: 1. add info to the data manager
 *           2. query data then verify
 */
HWTEST_F(BmsDataMgrTest, QueryAbilityInfo_0100, Function | SmallTest | Level0)
{
    InnerBundleInfo info1;
    BundleInfo bundleInfo1;
    bundleInfo1.name = BUNDLE_NAME;
    bundleInfo1.applicationInfo.name = APP_NAME;
    bundleInfo1.applicationInfo.bundleName = BUNDLE_NAME;
    ApplicationInfo applicationInfo1;
    applicationInfo1.name = BUNDLE_NAME;

    AbilityInfo abilityInfo = GetDefaultAbilityInfo();
    bundleInfo1.abilityInfos.push_back(abilityInfo);
    info1.SetBaseBundleInfo(bundleInfo1);
    info1.SetBaseApplicationInfo(applicationInfo1);
    info1.InsertAbilitiesInfo(BUNDLE_NAME + PACKAGE_NAME + ABILITY_NAME, abilityInfo);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    EXPECT_TRUE(ret1);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    EXPECT_TRUE(ret2);
    bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info1);
    EXPECT_TRUE(ret3);

    Want want;
    ElementName name;
    name.SetAbilityName(ABILITY_NAME);
    name.SetBundleName(BUNDLE_NAME);
    want.SetElement(name);

    AbilityInfo abilityInfo2;
    bool ret4 = dataMgr->QueryAbilityInfo(want, abilityInfo2);
    EXPECT_TRUE(ret4);

    Parcel parcel;
    parcel.WriteParcelable(&abilityInfo);
    std::unique_ptr<AbilityInfo> abilityInfo3;
    abilityInfo3.reset(parcel.ReadParcelable<AbilityInfo>());

    EXPECT_EQ(abilityInfo3->package, abilityInfo2.package);
    EXPECT_EQ(abilityInfo3->name, abilityInfo2.name);
    EXPECT_EQ(abilityInfo3->bundleName, abilityInfo2.bundleName);
    EXPECT_EQ(abilityInfo3->applicationName, abilityInfo2.applicationName);
    EXPECT_EQ(abilityInfo3->deviceId, abilityInfo2.deviceId);
    EXPECT_EQ(abilityInfo3->label, abilityInfo2.label);
    EXPECT_EQ(abilityInfo3->description, abilityInfo2.description);
    EXPECT_EQ(abilityInfo3->iconPath, abilityInfo2.iconPath);
    EXPECT_EQ(abilityInfo3->visible, abilityInfo2.visible);
    EXPECT_EQ(abilityInfo3->kind, abilityInfo2.kind);
    EXPECT_EQ(abilityInfo3->type, abilityInfo2.type);
    EXPECT_EQ(abilityInfo3->orientation, abilityInfo2.orientation);
    EXPECT_EQ(abilityInfo3->launchMode, abilityInfo2.launchMode);
    EXPECT_EQ(abilityInfo3->codePath, abilityInfo2.codePath);
    EXPECT_EQ(abilityInfo3->resourcePath, abilityInfo2.resourcePath);
    EXPECT_EQ(abilityInfo3->libPath, abilityInfo2.libPath);

    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: QueryAbilityInfo_0200
 * @tc.name: QueryAbilityInfo
 * @tc.desc: 1. add info to the data manager
 *           2. query data then verify
 */
HWTEST_F(BmsDataMgrTest, QueryAbilityInfo_0200, Function | SmallTest | Level0)
{
    Want want;
    ElementName name;
    name.SetAbilityName(ABILITY_NAME);
    want.SetElement(name);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    AbilityInfo abilityInfo;
    bool ret = dataMgr->QueryAbilityInfo(want, abilityInfo);
    EXPECT_FALSE(ret);
}

/**
 * @tc.number: QueryAbilityInfo_0300
 * @tc.name: QueryAbilityInfo
 * @tc.desc: 1. add info to the data manager
 *           2. query data then verify
 */
HWTEST_F(BmsDataMgrTest, QueryAbilityInfo_0300, Function | SmallTest | Level0)
{
    Want want;
    ElementName element1;
    EXPECT_EQ("//", element1.GetURI());

    element1.SetDeviceID(DEVICE_ID);
    EXPECT_EQ(DEVICE_ID, element1.GetDeviceID());

    element1.SetBundleName(BUNDLE_NAME);
    EXPECT_EQ(BUNDLE_NAME, element1.GetBundleName());

    element1.SetAbilityName(ABILITY_NAME);
    EXPECT_EQ(ABILITY_NAME, element1.GetAbilityName());
    EXPECT_EQ(DEVICE_ID + "/" + BUNDLE_NAME + "/" + ABILITY_NAME, element1.GetURI());

    ElementName element2(DEVICE_ID, BUNDLE_NAME, ABILITY_NAME);
    EXPECT_EQ(DEVICE_ID + "/" + BUNDLE_NAME + "/" + ABILITY_NAME, element2.GetURI());

    bool equal = (element2 == element1);
    EXPECT_TRUE(equal);

    Parcel parcel;
    parcel.WriteParcelable(&element1);
    std::unique_ptr<ElementName> newElement;
    newElement.reset(parcel.ReadParcelable<ElementName>());
    EXPECT_EQ(newElement->GetDeviceID(), element1.GetDeviceID());
    EXPECT_EQ(newElement->GetBundleName(), element1.GetBundleName());
    EXPECT_EQ(newElement->GetAbilityName(), element1.GetAbilityName());

    want.SetElement(element1);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    AbilityInfo abilityInfo;
    bool ret = dataMgr->QueryAbilityInfo(want, abilityInfo);
    EXPECT_FALSE(ret);
}

/**
 * @tc.number: GetApplicationInfo_0100
 * @tc.name: GetApplicationInfo
 * @tc.desc: 1. add info to the data manager
 *           2. query data then verify
 */
HWTEST_F(BmsDataMgrTest, GetApplicationInfo_0100, Function | SmallTest | Level0)
{
    InnerBundleInfo info1;
    BundleInfo bundleInfo1;
    bundleInfo1.name = BUNDLE_NAME;
    bundleInfo1.applicationInfo.name = APP_NAME;
    bundleInfo1.applicationInfo.bundleName = BUNDLE_NAME;
    ApplicationInfo applicationInfo1;
    applicationInfo1.name = BUNDLE_NAME;
    info1.SetBaseBundleInfo(bundleInfo1);
    info1.SetBaseApplicationInfo(applicationInfo1);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
    bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
    bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info1);

    ApplicationInfo appInfo;
    bool ret4 = dataMgr->GetApplicationInfo(APP_NAME, ApplicationFlag::GET_BASIC_APPLICATION_INFO, USERID, appInfo);
    std::string name = appInfo.name;
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);
    EXPECT_EQ(name, APP_NAME);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
    dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
}

/**
 * @tc.number: GetApplicationInfo_0200
 * @tc.name: GetApplicationInfo
 * @tc.desc: 1. add info to the data manager
 *           2. query data then verify
 */
HWTEST_F(BmsDataMgrTest, GetApplicationInfo_0200, Function | SmallTest | Level0)
{
    ApplicationInfo appInfo;
    appInfo.name = APP_NAME;
    appInfo.bundleName = BUNDLE_NAME;
    appInfo.deviceId = DEVICE_ID;

    Parcel parcel;
    parcel.WriteParcelable(&appInfo);
    std::unique_ptr<ApplicationInfo> appInfo2;
    appInfo2.reset(parcel.ReadParcelable<ApplicationInfo>());
    EXPECT_EQ(appInfo.name, appInfo2->name);
    EXPECT_EQ(appInfo.bundleName, appInfo2->bundleName);
    EXPECT_EQ(appInfo.deviceId, appInfo2->deviceId);
    auto dataMgr = GetDataMgr();
    ASSERT_NE(dataMgr, nullptr);
    ApplicationInfo appInfo3;
    bool ret = dataMgr->GetApplicationInfo(BUNDLE_NAME, ApplicationFlag::GET_BASIC_APPLICATION_INFO, USERID, appInfo3);
    EXPECT_FALSE(ret);

    EXPECT_NE(appInfo.name, appInfo3.name);
    EXPECT_NE(appInfo.bundleName, appInfo3.bundleName);
    EXPECT_NE(appInfo.deviceId, appInfo3.deviceId);
}