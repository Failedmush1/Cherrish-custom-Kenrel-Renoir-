#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/types.h>

struct disp_event { int disp_id; int type; int length; };
struct disp_feature { int placeholder; };

int mi_disp_feature_init(void) { return 0; }
int mi_disp_feature_deinit(void) { return 0; }
int mi_disp_feature_attach_display(void *display, int disp_id, int intf_type) { return 0; }
int mi_disp_feature_detach_display(int disp_id) { return 0; }
void mi_disp_feature_sysfs_notify(int disp_id, int event) { }
void mi_disp_feature_event_notify(struct disp_event *event, u8 *payload) { }
struct disp_feature *mi_get_disp_feature(void) { return NULL; }
void mi_disp_printk_utc(const char *level, const char *format, ...) {
    va_list args; va_start(args, format); vprintk(format, args); va_end(args);
}

const char *get_display_power_mode_name(int power_mode) {
    return "unknown";
}

int mi_dsi_panel_write_cmd_set(void *panel, void *cmd_sets) {
    return 0;
}

int mi_dsi_panel_lhbm_set(void *panel) {
    return 0;
}

int mi_dsi_panel_update_vdc_param(void *panel) {
    return 0;
}

int mi_dsi_panel_read_and_update_flatmode_param(void *panel) {
    return 0;
}

int mi_dsi_panel_update_lhbm_param(void *panel) {
    return 0;
}

int mi_dsi_panel_read_and_update_dc_param(void *panel) {
    return 0;
}

void mi_dsi_display_update_backlight(void *display) { }
