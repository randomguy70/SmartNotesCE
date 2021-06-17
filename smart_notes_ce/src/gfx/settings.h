#ifndef settings_include_file
#define settings_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define settings_width 20
#define settings_height 20
#define settings_size 402
#define settings ((gfx_sprite_t*)settings_data)
extern unsigned char settings_data[402];

#ifdef __cplusplus
}
#endif

#endif
