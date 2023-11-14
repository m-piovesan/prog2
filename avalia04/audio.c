#include "audio.h"

void audio_init() {
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(128);

    sample_shot = al_load_sample("./media/sound/shot.flac");
    must_init(sample_shot, "shot sample");

    sample_explode[0] = al_load_sample("./media/sound/explode1.flac");
    must_init(sample_explode[0], "explode[0] sample");
    sample_explode[1] = al_load_sample("./media/sound/explode2.flac");
    must_init(sample_explode[1], "explode[1] sample");
}

void audio_deinit() {
    al_destroy_sample(sample_shot);
    al_destroy_sample(sample_explode[0]);
    al_destroy_sample(sample_explode[1]);
}