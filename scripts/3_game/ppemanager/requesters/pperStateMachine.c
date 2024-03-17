class PPERequester_anzwStateMachine extends PPERequester_GameplayBase
{
	enum EffectState
	{
		START,
		FADE_IN,
		RUN,
		FADE_OUT,
		STOP
	}
	protected EffectState m_State = EffectState.STOP;

	protected vector m_StartRGB = vector.Zero;
	protected float m_ExpValue = 0;
	protected float m_BlurValue = 0;
	protected float m_AccumulatedTime = 0;

	const float FADE_TIME = 1;
	const float RUN_TIME = 5;

	// the end result is 1 - the value set here
	const float R_TARGET = 0;
	const float G_TARGET = 1;
	const float B_TARGET = 0.2;

	const float EXP_TARGET = 5;
	const float BLUR_TARGET = 0.3;

	EffectState GetEffectState() { return m_State; }
	
	override protected void OnStart( Param par = null )
	{
		super.OnStart( par );
		m_State = StateProcessing(EffectState.START, 0);		
	}
	
	override protected void OnUpdate( float delta )
	{
		super.OnUpdate( delta );

		m_State = StateProcessing(m_State, delta);
	}
	
	override void OnStop(Param par = null)
	{
		super.OnStop(par);
	}

	EffectState StateProcessing(EffectState state, float delta = -1.0)
	{
		if (delta == -1.0)
			state = EffectState.STOP;

		switch(state)
		{
			case EffectState.START:
				m_AccumulatedTime = 0;
				m_ExpValue = 0;
				m_BlurValue = 0;
				state = EffectState.FADE_IN;
				break;

			case EffectState.FADE_IN:
				m_AccumulatedTime += delta;
				FadeIn( m_AccumulatedTime / FADE_TIME );
				if ( m_AccumulatedTime >= FADE_TIME )
				{
					m_AccumulatedTime = 0;
					state = EffectState.RUN;
				}
				break;

			case EffectState.RUN:
				m_AccumulatedTime += delta;
				Run( m_AccumulatedTime / RUN_TIME )
				if ( m_AccumulatedTime >= RUN_TIME )
				{
					m_AccumulatedTime = 0;
					state = EffectState.FADE_OUT;
				}
				break;

			case EffectState.FADE_OUT:
				m_AccumulatedTime += delta;
				FadeOut( m_AccumulatedTime / FADE_TIME );
				if ( m_AccumulatedTime >= FADE_TIME )
					state = EffectState.STOP;
				break;

			case EffectState.STOP:
				Stop();
				break;

			default:
				break;
		}
		return state;
	}

	protected void FadeIn(float time)
	{
		m_ExpValue = 0 + FadeColourMult( 0, 1, time ) * EXP_TARGET;
		m_BlurValue = 0 + FadeColourMult( 0, 1, time ) * BLUR_TARGET;

		m_StartRGB[0] = 1 - FadeColourMult( 0, 1, time ) * R_TARGET;
		m_StartRGB[1] = 1 - FadeColourMult( 0, 1, time ) * G_TARGET;
		m_StartRGB[2] = 1 - FadeColourMult( 0, 1, time ) * B_TARGET;
			
		SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_COLORIZATIONCOLOR,{m_StartRGB[0], m_StartRGB[1], m_StartRGB[2], 0.0},PPEGlow.L_23_TOXIC_TINT,PPOperators.MULTIPLICATIVE);
		SetTargetValueFloat(PPEExceptions.EXPOSURE,PPEExposureNative.PARAM_INTENSITY,false,m_ExpValue,PPEExposureNative.L_0_DEATH,PPOperators.SET);
		SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,m_BlurValue,PPEGaussFilter.L_0_SHOCK,PPOperators.ADD_RELATIVE);
	}

	protected void Run(float time)
	{	
		SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_COLORIZATIONCOLOR,{m_StartRGB[0], m_StartRGB[1], m_StartRGB[2], 0.0},PPEGlow.L_23_TOXIC_TINT,PPOperators.MULTIPLICATIVE);
		SetTargetValueFloat(PPEExceptions.EXPOSURE,PPEExposureNative.PARAM_INTENSITY,false,m_ExpValue,PPEExposureNative.L_0_DEATH,PPOperators.SET);
		SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,m_BlurValue,PPEGaussFilter.L_0_SHOCK,PPOperators.ADD_RELATIVE);
	}

	protected void FadeOut(float time)
	{
		m_ExpValue = EXP_TARGET - FadeColourMult( 0, EXP_TARGET, m_AccumulatedTime / FADE_TIME );
		m_BlurValue = BLUR_TARGET - FadeColourMult( 0, BLUR_TARGET, m_AccumulatedTime / FADE_TIME );

		m_StartRGB[0] = ( 1 - R_TARGET ) + FadeColourMult( 0, R_TARGET, time );
		m_StartRGB[1] = ( 1 - G_TARGET ) + FadeColourMult( 0, G_TARGET, time );
		m_StartRGB[2] = ( 1 - B_TARGET ) + FadeColourMult( 0, B_TARGET, time );
		
		SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_COLORIZATIONCOLOR,{m_StartRGB[0], m_StartRGB[1], m_StartRGB[2], 0.0},PPEGlow.L_23_TOXIC_TINT,PPOperators.MULTIPLICATIVE);
		SetTargetValueFloat(PPEExceptions.EXPOSURE,PPEExposureNative.PARAM_INTENSITY,false,m_ExpValue,PPEExposureNative.L_0_DEATH,PPOperators.SET);
		SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,m_BlurValue,PPEGaussFilter.L_0_SHOCK,PPOperators.ADD_RELATIVE);
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