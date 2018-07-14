// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018-2018 The H2O Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"
#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include <math.h>
#include <stdio.h>
#include "chainparams.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include <assert.h>
#include <inttypes.h>
#include <boost/assign/list_of.hpp>
#include "chainparamsseeds.h"


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}


static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "CoinDesk - May 11, 2018 - Huawei to Offer First Crypto Wallet App on Latest Smartphones";
    const CScript genesisOutputScript = CScript() << ParseHex("04ed4ae4b0f8333b772a227c3be3d211910b0a14d5057950e47ca8e598e39b65a54807dfbc2827e849ef1c575ff769ee3af9f372d84c8ecae5629518809b9e9176") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 131400; // 1/2 of the year
        consensus.nMasternodePaymentsStartBlock = 720;
        consensus.nMasternodePaymentsIncreaseBlock = 500000000; // not used
        consensus.nMasternodePaymentsIncreasePeriod = 500000000; // not used
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 500000000; // not used
        consensus.nBudgetPaymentsCycleBlocks = 50000;
        consensus.nBudgetPaymentsWindowBlocks = 1000;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 600000000; // not used
        consensus.nSuperblockCycle = 16616;
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 510;
        consensus.nMajorityRejectBlockOutdated = 750;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 0; // from the start
        consensus.BIP34Hash = uint256S("0x00000ed71b05737fed83319d001be6f2361f67052967c2fc05e9df5890d2b0e6");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 4 * 120; // 480 seconds
        consensus.nPowTargetSpacing = 120; // 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000; // Aug 9th, 2018

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa2;
        pchMessageStart[1] = 0xc4;
        pchMessageStart[2] = 0xcb;
        pchMessageStart[3] = 0x4a;
        vAlertPubKey = ParseHex("046801202c9a170b64782f18ae9e862749c0bc541809f064010feb957550adbb18f20305266cd81dd89be3bfb58e67e2f61e27a4426baed3ee0bb2d60fb0b22df0");
        nDefaultPort = 13355;
        nMaxTipAge = 6 * 60 * 60;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1526155200, 709788, 0x1e0ffff0, 1, 10 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000ed71b05737fed83319d001be6f2361f67052967c2fc05e9df5890d2b0e6"));
        assert(genesis.hashMerkleRoot == uint256S("0x759ae9bc6cbce7bdbe8633185a76ecc06e653d4e74d51862b993f403b25e9c91"));


//        vFixedSeeds.clear();
//        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("se1.h2ocore.org", "se1.h2ocore.org"));
        vSeeds.push_back(CDNSSeedData("se2.h2ocore.org", "se2.h2ocore.org"));
        vSeeds.push_back(CDNSSeedData("se3.h2ocore.org", "se3.h2ocore.org"));
        vSeeds.push_back(CDNSSeedData("se4.h2ocore.org", "se4.h2ocore.org"));
        vSeeds.push_back(CDNSSeedData("se5.h2ocore.org", "se5.h2ocore.org"));
        vSeeds.push_back(CDNSSeedData("se6.h2ocore.org", "se6.h2ocore.org"));
        vSeeds.push_back(CDNSSeedData("se7.h2ocore.org", "se7.h2ocore.org"));
        vSeeds.push_back(CDNSSeedData("se8.h2ocore.org", "se8.h2ocore.org"));
        vSeeds.push_back(CDNSSeedData("se9.h2ocore.org", "se9.h2ocore.org"));

        // H2O addresses start with 'H'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,41);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,30);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,224);
        // H2O BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // H2O BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // H2O BIP44 coin type is '5'
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x05).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "043ed47a1aeccaa993c2efa2e9ed351c59e43da97a1f69cff54d14d2aeaa93aced75416a675ff4818a4b23fe8c7712a7514f4956164eb89deb711cd791701a4800";
        strMasternodePaymentsPubKey = "043ed47a1aeccaa993c2efa2e9ed351c59e43da97a1f69cff54d14d2aeaa93aced75416a675ff4818a4b23fe8c7712a7514f4956164eb89deb711cd791701a4800";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (    0, uint256S("0x00000ed71b05737fed83319d001be6f2361f67052967c2fc05e9df5890d2b0e6"))
            (10000, uint256("0x000000000007b8c12cd833522b64e6b7b2daf49fc465bd769a2f7f8848a90d8f"))
            (20000, uint256("0x00000000000fc79fa7456e4c74c6c29b38718eae06dd9c56f85c09689651f9e8"))
            (30000, uint256("0x000000000020e66fcc803de1ebde08757ff31ea67a075052f0f5da68304eb891"))
            (35812, uint256("0x0000000000436b174658889f2a6d3d5080c45c3cb13f179c848efd91e424b887")),
            1530718878, // * UNIX timestamp of last checkpoint block
            53982,    //  total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            2400        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 131400; // 1/2 of the year
        consensus.nMasternodePaymentsStartBlock = 720; //
        consensus.nMasternodePaymentsIncreaseBlock = 500000000; // not used
        consensus.nMasternodePaymentsIncreasePeriod = 500000000; // not used
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 500000000; // not used
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 600000000; // not used
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 0; // from the start
        consensus.BIP34Hash = uint256S("0x0000038134bd91836572cdfd34b63cda67344dae95f0cfba1ee6f6a259d785ba");
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 4 * 120; // 480 seconds
        consensus.nPowTargetSpacing = 120; // 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1502280000; // Aug 9th, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1533816000; // Aug 9th, 2018

        pchMessageStart[0] = 0x2a;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0x6c;
        vAlertPubKey = ParseHex("04723eaccf0aad74336d9538d89c80a599c407cbc5a1da6e5e39c83d35d1045e3da90c059551ae52bdf31d409d98f1f74828939bac5629b3de7d2988a28a6253a2");
        nDefaultPort = 14466;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nPruneAfterHeight = 1000;
        nMasternodeCollateralAmt=15000; //masternode collateral

        genesis = CreateGenesisBlock(1526155205, 888158, 0x1e0ffff0, 1, 10 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000038134bd91836572cdfd34b63cda67344dae95f0cfba1ee6f6a259d785ba"));
        assert(genesis.hashMerkleRoot == uint256S("0x759ae9bc6cbce7bdbe8633185a76ecc06e653d4e74d51862b993f403b25e9c91"));

        vFixedSeeds.clear();
        vSeeds.clear();
//      vSeeds.push_back(CDNSSeedData("h2oproject.co",  "set1.h2oproject.co"));
//      vSeeds.push_back(CDNSSeedData("h2oproject.co",  "set2.h2oproject.co"));

        // Testnet H2O addresses start with 'n'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,20);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        // Testnet H2O BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet H2O BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet H2O BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04a38cdfb6d7ac17632b0fe6a698ca4861ffddc258299fd24d18891a5deda36a5b4c5c81ca953bb8e9d7e78567b9536a0b8b33dedfa758037cff910d81219754a5";
        strMasternodePaymentsPubKey = "04a38cdfb6d7ac17632b0fe6a698ca4861ffddc258299fd24d18891a5deda36a5b4c5c81ca953bb8e9d7e78567b9536a0b8b33dedfa758037cff910d81219754a5";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x0000038134bd91836572cdfd34b63cda67344dae95f0cfba1ee6f6a259d785ba")),
	     1526155205, // * UNIX timestamp of last checkpoint block
            0,     // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500         // * estimated number of transactions per day after checkpoint
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4 * 120; // 480 seconds
        consensus.nPowTargetSpacing = 120; // 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0xc1;
        pchMessageStart[1] = 0x2a;
        pchMessageStart[2] = 0xd1;
        pchMessageStart[3] = 0xab;
        nMaxTipAge = 30 * 24 * 60 * 60;
        nDefaultPort = 15577;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1526155210, 0, 0x207fffff, 1, 10 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x60126ff21de6824d16e872ec03ee37d89fb22f743e5ccc4730c90b9e4ec397f8"));
        assert(genesis.hashMerkleRoot == uint256S("0x759ae9bc6cbce7bdbe8633185a76ecc06e653d4e74d51862b993f403b25e9c91"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x60126ff21de6824d16e872ec03ee37d89fb22f743e5ccc4730c90b9e4ec397f8")),
            0,
            0,
            0
        };
        // Regtest H2O addresses start with 'n'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,20);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        // Regtest H2O BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest H2O BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Regtest H2O BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
