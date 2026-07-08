#include "script/userScript.h"
#include "scene/sceneManager.h"
#include "scene/object.h"

namespace P64::Script::C1EF59BA6590AFD6
{
  P64_DATA(
    float yaw;
    float pitch;
  );

  void init(Object& obj, Data* data)
  {
    data->yaw = 0.0f;
    data->pitch = 0.0f;
  }

  void update(Object& obj, Data* data, float deltaTime)
  {
    const auto input = joypad_get_inputs(JOYPAD_PORT_1);

    constexpr float MOVE_SPEED = 250.0f;
    constexpr float LOOK_SPEED = 1.8f;
    constexpr float STICK_MAX = 80.0f;
    constexpr float MAX_PITCH = 1.35f;

    // Arrow keys, mapped through Ares to the N64 C-buttons.
    if(input.btn.c_left)
    {
      data->yaw -= LOOK_SPEED * deltaTime;
    }

    if(input.btn.c_right)
    {
      data->yaw += LOOK_SPEED * deltaTime;
    }

    if(input.btn.c_up)
    {
      data->pitch += LOOK_SPEED * deltaTime;
    }

    if(input.btn.c_down)
    {
      data->pitch -= LOOK_SPEED * deltaTime;
    }

    if(data->pitch > MAX_PITCH)
    {
      data->pitch = MAX_PITCH;
    }

    if(data->pitch < -MAX_PITCH)
    {
      data->pitch = -MAX_PITCH;
    }

    const float yawSin = fm_sinf(data->yaw);
    const float yawCos = fm_cosf(data->yaw);
    const float pitchSin = fm_sinf(data->pitch);
    const float pitchCos = fm_cosf(data->pitch);

    // Full direction used for looking.
    const fm_vec3_t lookForward = {
      yawSin * pitchCos,
      pitchSin,
      -yawCos * pitchCos
    };

    // Horizontal directions used for walking.
    const fm_vec3_t moveForward = {
      yawSin,
      0.0f,
      -yawCos
    };

    const fm_vec3_t moveRight = {
      yawCos,
      0.0f,
      yawSin
    };

    // WASD is mapped to the N64 analog stick in Ares.
    const float forwardInput =
      static_cast<float>(input.stick_y) / STICK_MAX;

    const float rightInput =
      static_cast<float>(input.stick_x) / STICK_MAX;

    const fm_vec3_t movement =
      moveForward * forwardInput +
      moveRight * rightInput;

    obj.pos += movement * MOVE_SPEED * deltaTime;

    auto& camera = obj.getScene().getActiveCamera();
    camera.setLookAt(
      obj.pos,
      obj.pos + lookForward * 100.0f
    );
  }
}
