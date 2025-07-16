float get_boost(float rpm, float throttle, float previous_boost, float max_boost, float inertia) {
    const float base_boost = -0.3f;
    const float turbo_start_rpm = 1800.0f;
    const float turbo_full_rpm = 5000.0f;

    if (throttle < 0.05f) {
        return previous_boost + (base_boost - previous_boost) * inertia;
    }

    float rpm_factor = (rpm - turbo_start_rpm) / (turbo_full_rpm - turbo_start_rpm);
    if (rpm_factor < 0.0f) rpm_factor = 0.0f;
    if (rpm_factor > 1.0f) rpm_factor = 1.0f;

    float target_boost = base_boost + throttle * rpm_factor * (max_boost - base_boost);
    return previous_boost + (target_boost - previous_boost) * inertia;
}

// Fonction température eau réaliste
float get_WaterTemp(float current_temp, float rpm, float throttle, float ambient_temp, float time_seconds) {
    float heating = 0.02f * throttle * (rpm / 1000.0f);
    float cooling = 0.001f;

    float target_temp = ambient_temp + 65.0f + throttle * 35.0f;

    current_temp += (target_temp - current_temp) * heating;
    current_temp -= (current_temp - ambient_temp) * cooling;

    // Clamp température raisonnable
    if (current_temp < ambient_temp) current_temp = ambient_temp;
    if (current_temp > 110.0f) current_temp = 110.0f;

    return current_temp;
}

// Pression huile simplifiée
float get_oil_pressure(float rpm, float previous_pressure, float max_pressure, float inertia) {
    float target_pressure = (rpm / 7000.0f) * max_pressure;
    if (target_pressure < 1.0f) target_pressure = 1.0f;  // Minimum pression huile

    return previous_pressure + (target_pressure - previous_pressure) * inertia;
}

float update_rpm(float rpm, float throttle) {
    const float rpm_idle = 800.0f;
    const float rpm_max = 7000.0f;

    float rpm_target = rpm_idle + throttle * (rpm_max - rpm_idle);
    rpm += (rpm_target - rpm) * 0.05f; 

    return rpm;
}