#include"camera.h"

void cameraMovement(GS* gs){
    // gs->camera.target = (Vector2){gs->player.position.x,0.0f};
    float player_center_x = gs->player.position.x  + gs->player.collisionOffset.x + gs->player.width / 2.0f;
    float camera_right_pos = gs->camera.target.x + camera_half_deadzone;

    if(player_center_x>camera_right_pos){
        gs->camera.target = (Vector2){player_center_x-camera_half_deadzone,0.0f};
    }
}
