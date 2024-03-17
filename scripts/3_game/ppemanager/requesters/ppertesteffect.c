class PPERequester_anzwTest extends PPERequester_GameplayBase
{
	protected float m_ExpValue = 0;
	protected float m_BlurValue = 0;
	

	protected vector m_StartRGB = vector.Zero;
	protected float m_AccumulatedTime = 0;
	protected bool m_FadeIn = false;
	protected bool m_FadeOut = false;

	protected bool m_SpeedIncrease = true;
	
	protected float m_FadeTime = 1;

	// the end result is 1 - the value set here
	const float R_TARGET = 0;
	const float G_TARGET = 1;
	const float B_TARGET = 0.2;

	const float EXP_TARGET = 5;
	const float BLUR_TARGET = 0.3;
	
	override protected void OnStart( Param par = null )
	{
		super.OnStart( par );
		
		m_AccumulatedTime = 0;
		m_ExpValue = 0;
		m_BlurValue = 0;

		m_SpeedIncrease = true;
		
		m_FadeIn = true;
		m_FadeOut = false;

		
		//SetTargetValueFloat(PostProcessEffectType.FilmGrain,PPEFilmGrain.PARAM_SHARPNESS,false,1.0,PPEFilmGrain.L_1_TOXIC_TINT,PPOperators.SET);
		//SetTargetValueFloat(PostProcessEffectType.FilmGrain,PPEFilmGrain.PARAM_GRAINSIZE,false,10.0,PPEFilmGrain.L_2_TOXIC_TINT,PPOperators.SET);
		//SetTargetValueFloat(PPEExceptions.NONE,PPELightIntensityParamsNative.PARAM_LIGHT_MULT,false,3.0,PPELightIntensityParamsNative.L_0_TOXIC_TINT,PPOperators.SET);
		//SetTargetValueFloat(PPEExceptions.NONE,PPELightIntensityParamsNative.PARAM_NOISE_MULT,false,1.0,PPELightIntensityParamsNative.L_1_TOXIC_TINT,PPOperators.SET);


		//m_ExpValue = Param1<float>.Cast(par).param1;
		//Print("PPERequester_DeathDarkening | level: " + m_ExpValue);
		//m_ExpValue = Easing.EaseInCubic(m_ExpValue);
		//m_ExpValue = Math.Lerp(0,-10,m_ExpValue);
		//Print("PPERequester_DeathDarkening | value: " + value);
		
		
	}
	
	override protected void OnUpdate( float delta )
	{
		super.OnUpdate( delta );

				
		
		if ( m_FadeIn && m_AccumulatedTime <= m_FadeTime )
		{
			m_AccumulatedTime += delta;

			

			m_ExpValue = 0 + FadeColourMult( 0, 1, m_AccumulatedTime / m_FadeTime ) * EXP_TARGET;
			m_BlurValue = 0 + FadeColourMult( 0, 1, m_AccumulatedTime / m_FadeTime ) * BLUR_TARGET;
		
			m_StartRGB[0] = 1 - FadeColourMult( 0, 1, m_AccumulatedTime / m_FadeTime ) * R_TARGET;
			m_StartRGB[1] = 1 - FadeColourMult( 0, 1, m_AccumulatedTime / m_FadeTime ) * G_TARGET;
			m_StartRGB[2] = 1 - FadeColourMult( 0, 1, m_AccumulatedTime / m_FadeTime ) * B_TARGET;
			
			SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_COLORIZATIONCOLOR,{m_StartRGB[0], m_StartRGB[1], m_StartRGB[2], 0.0},PPEGlow.L_23_TOXIC_TINT,PPOperators.MULTIPLICATIVE);
			SetTargetValueFloat(PPEExceptions.EXPOSURE,PPEExposureNative.PARAM_INTENSITY,false,m_ExpValue,PPEExposureNative.L_0_DEATH,PPOperators.SET);
			//blur
			SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,m_BlurValue,PPEGaussFilter.L_0_SHOCK,PPOperators.ADD_RELATIVE);
		}
		
		if ( m_FadeIn && m_AccumulatedTime > m_FadeTime )
		{
			m_AccumulatedTime = 0;
			m_FadeIn = false;
			m_FadeOut = true;
		}
		
		if ( m_FadeOut )
		{
			if (m_AccumulatedTime <= m_FadeTime)
			{
				m_AccumulatedTime += delta;

		
				
				m_ExpValue = EXP_TARGET - FadeColourMult( 0, EXP_TARGET, m_AccumulatedTime / m_FadeTime );
				m_BlurValue = BLUR_TARGET - FadeColourMult( 0, BLUR_TARGET, m_AccumulatedTime / m_FadeTime );

				m_StartRGB[0] = ( 1 - R_TARGET ) + FadeColourMult( 0, R_TARGET, m_AccumulatedTime / m_FadeTime );
				m_StartRGB[1] = ( 1 - G_TARGET ) + FadeColourMult( 0, G_TARGET, m_AccumulatedTime / m_FadeTime );
				m_StartRGB[2] = ( 1 - B_TARGET ) + FadeColourMult( 0, B_TARGET, m_AccumulatedTime / m_FadeTime );
				
				SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_COLORIZATIONCOLOR,{m_StartRGB[0], m_StartRGB[1], m_StartRGB[2], 0.0},PPEGlow.L_23_TOXIC_TINT,PPOperators.MULTIPLICATIVE);
				SetTargetValueFloat(PPEExceptions.EXPOSURE,PPEExposureNative.PARAM_INTENSITY,false,m_ExpValue,PPEExposureNative.L_0_DEATH,PPOperators.SET);
				SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,m_BlurValue,PPEGaussFilter.L_0_SHOCK,PPOperators.ADD_RELATIVE);
			}
			else
			{
				OnStart();
				
				//Stop(); //proper termination after a fadeout
			}
		}
	}
	
	override void OnStop(Param par = null)
	{
		m_FadeIn = false;
		m_FadeOut = false;
		Param1<bool> p;
		
		if (par && Class.CastTo(p,par))
			m_FadeOut = p.param1;
		
		m_AccumulatedTime = 0;
		
		super.OnStop(par);
	}
	
	// Lerped multiplier for RGBA values
	protected float FadeColourMult( float x, float y, float deltaT )
	{
		float output;
		output = Math.Lerp( x, y, deltaT );
		Easing.EaseInOutSine( output );
		return output;
	}
}