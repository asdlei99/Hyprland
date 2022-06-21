#pragma once

#define CONFIG_MANAGER_H

#include <map>
#include "../debug/Log.hpp"
#include <unordered_map>
#include "../defines.hpp"
#include <vector>
#include <deque>
#include <algorithm>
#include <regex>
#include "../Window.hpp"

#include "defaultConfig.hpp"

#define STRVAL_EMPTY "[[EMPTY]]"

struct SConfigValue {
    int64_t intValue = -1;
    float floatValue = -1;
    std::string strValue = "";
};

struct SMonitorRule {
    std::string name = "";
    Vector2D    resolution = Vector2D(1280,720);
    Vector2D    offset = Vector2D(0,0);
    float       scale = 1;
    float       refreshRate = 60;
    std::string defaultWorkspace = "";
    bool        disabled = false;
    wl_output_transform transform = WL_OUTPUT_TRANSFORM_NORMAL;
};

struct SMonitorAdditionalReservedArea {
    int         top = 0;
    int         bottom = 0;
    int         left = 0;
    int         right = 0;
};

struct SWindowRule {
    std::string szRule;
    std::string szValue;
};

class CConfigManager {
public:
    CConfigManager();

    void                tick();
    void                init();

    int                 getInt(std::string);
    float               getFloat(std::string);
    std::string         getString(std::string);
    void                setFloat(std::string, float);
    void                setInt(std::string, int);
    void                setString(std::string, std::string);

    SConfigValue*       getConfigValuePtr(std::string);

    SMonitorRule        getMonitorRuleFor(std::string);

    std::vector<SWindowRule> getMatchingRules(CWindow*);

    std::unordered_map<std::string, SMonitorAdditionalReservedArea> m_mAdditionalReservedAreas;

    // no-op when done.
    void                dispatchExecOnce();

    void                performMonitorReload();
    bool                m_bWantsMonitorReload = false;
    bool                m_bForceReload = false;

    std::string         parseKeyword(const std::string&, const std::string&, bool dynamic = false);

private:
    std::deque<std::string>                       configPaths; // stores all the config paths
    std::unordered_map<std::string, time_t>       configModifyTimes; // stores modify times
    std::unordered_map<std::string, std::string>  configDynamicVars; // stores dynamic vars declared by the user
    std::unordered_map<std::string, SConfigValue> configValues;

    std::string                                   configCurrentPath;

    std::string currentCategory = "";  // For storing the category of the current item

    std::string parseError = "";  // For storing a parse error to display later

    bool isFirstLaunch = true;  // For exec-once

    std::deque<SMonitorRule> m_dMonitorRules;
    std::deque<SWindowRule> m_dWindowRules;

    bool firstExecDispatched = false;
    std::deque<std::string> firstExecRequests;

    // internal methods
    void                setDefaultVars();

    void                applyUserDefinedVars(std::string&, const size_t);
    void                loadConfigLoadVars();
    SConfigValue        getConfigValueSafe(std::string);
    void                parseLine(std::string&);
    void                configSetValueSafe(const std::string&, const std::string&);
    void                handleRawExec(const std::string&, const std::string&);
    void                handleMonitor(const std::string&, const std::string&);
    void                handleBind(const std::string&, const std::string&, bool locked = false);
    void                handleUnbind(const std::string&, const std::string&);
    void                handleWindowRule(const std::string&, const std::string&);
    void                handleDefaultWorkspace(const std::string&, const std::string&);
    void                handleBezier(const std::string&, const std::string&);
    void                handleAnimation(const std::string&, const std::string&);
    void                handleSource(const std::string&, const std::string&);
};

inline std::unique_ptr<CConfigManager> g_pConfigManager;