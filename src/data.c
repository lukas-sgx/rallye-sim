float get_boost(float rpm, float throttle, float boost, float boostMax, float boostSpool) {
    float targetBoost;

    if (rpm < 1500.0f) {
        targetBoost = -0.4f + throttle * 0.2f;
    } else {
        float rpmFactor = (rpm - 1500.0f) / 3000.0f;
        if (rpmFactor > 1.0f) rpmFactor = 1.0f;
        if (rpmFactor < 0.0f) rpmFactor = 0.0f;
        targetBoost = throttle * boostMax * rpmFactor;
    }

    boost += (targetBoost - boost) * boostSpool;
    return boost;
}

float get_WaterTemp(float T_old, float rpm, float throttle, float T_amb, float dt) {
    float a = 0.0001f; 
    float b = 0.01f;

    float heat = a * rpm * throttle;
    float cool = b * (T_old - T_amb);

    float T_new = T_old + (heat - cool) * dt;

    if (T_new < T_amb) T_new = T_amb;
    if (T_new > 120.0f) T_new = 120.0f; 

    return T_new;
}
