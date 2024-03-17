class azw_EffectTest extends ItemBase
{

    void azw_EffectTest()
    {
    }

    void ~azw_EffectTest()
    {
    }

    override void EEInit()
	{
		PPERequesterBank.GetRequester(PPERequester_anzwTest).Start();
	}

}