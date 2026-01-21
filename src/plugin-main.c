#include <obs-module.h>
#include <string.h>

// 插件元数据
OBS_DECLARE_MODULE()
OBS_MODULE_AUTHOR("Gemini Partner")
const char *obs_module_name() { return "Global Media Fixer (C Version)"; }

// 检查并修复单个源的函数
static bool check_and_fix_source(void *unused, obs_source_t *source)
{
    if (!source) return true;

    const char *id = obs_source_get_unversioned_id(source);
    
    // 只处理媒体源 (ffmpeg_source)
    if (id && strcmp(id, "ffmpeg_source") == 0) {
        obs_data_t *settings = obs_source_get_settings(source);
        if (settings) {
            bool hw = obs_data_get_bool(settings, "hw_decode");
            bool cl = obs_data_get_bool(settings, "close_when_inactive");

            // 如果设置不符合要求，则强制修改
            if (!hw || !cl) {
                obs_data_set_bool(settings, "hw_decode", true);
                obs_data_set_bool(settings, "close_when_inactive", true);
                obs_data_set_bool(settings, "restart_on_active", true);
                
                obs_source_update(source, settings);
            }
            obs_data_release(settings);
        }
    }
    
    return true; // 继续枚举下一个源
    (void)unused;
}

// 计时器回调：每隔 2 秒扫描全场
static void timer_tick(void *param, uint64_t tick)
{
    static int frame_count = 0;
    // 假设 OBS 运行在 60fps，120 帧约为 2 秒
    if (++frame_count >= 120) {
        frame_count = 0;
        obs_enum_sources(check_and_fix_source, NULL);
    }
    (void)param;
    (void)tick;
}

// 插件加载时的入口
bool obs_module_load(void)
{
    // 注册视频渲染钩子作为计时器
    obs_add_tick_callback(timer_tick, NULL);
    return true;
}

// 插件卸载
void obs_module_unload(void)
{
    obs_remove_tick_callback(timer_tick, NULL);
}
