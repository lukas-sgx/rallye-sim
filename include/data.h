float get_boost(float rpm, float throttle, float boost, float boostMax, float boostSpool);
float get_WaterTemp(float T_old, float rpm, float throttle, float T_amb, float dt);
float get_oil_pressure(float rpm, float oilPressure, float maxOilPressure, float oilSpool);