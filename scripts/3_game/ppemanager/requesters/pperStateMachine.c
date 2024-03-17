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
	protected float m_AccumulateddeltaT = 0;

	const float FADE_IN_TIME = 0.5;
	const float FADE_OUT_TIME = 10;
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
	
	override void OnStop( Param par = null )
	{
		super.OnStop(par);
	}

	protected EffectState StateProcessing( EffectState state, float delta )
	{
		switch(state)
		{
			case EffectState.START:
				m_AccumulateddeltaT = 0;
				m_ExpValue = 0;
				m_BlurValue = 0;
				state = EffectState.FADE_IN;
				break;

			case EffectState.FADE_IN:
				m_AccumulateddeltaT += delta;
				FadeIn( m_AccumulateddeltaT / FADE_IN_TIME );

				if ( m_AccumulateddeltaT >= FADE_IN_TIME )
				{
					m_AccumulateddeltaT = 0;
					state = EffectState.RUN;
				}
				break;

			case EffectState.RUN:
				m_AccumulateddeltaT += delta;
				Run( m_AccumulateddeltaT / RUN_TIME )

				if ( m_AccumulateddeltaT >= RUN_TIME )
				{
					m_AccumulateddeltaT = 0;
					state = EffectState.FADE_OUT;
				}
				break;

			case EffectState.FADE_OUT:
				m_AccumulateddeltaT += delta;
				FadeOut( m_AccumulateddeltaT / FADE_OUT_TIME );
				
				if ( m_AccumulateddeltaT >= FADE_OUT_TIME )
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

	protected void FadeIn( float deltaT )
	{
		m_ExpValue = 0 + FadeColourMult( 0, 1, deltaT ) * EXP_TARGET;
		m_BlurValue = 0 + FadeColourMult( 0, 1, deltaT ) * BLUR_TARGET;

		m_StartRGB[0] = 1 - FadeColourMult( 0, 1, deltaT ) * R_TARGET;
		m_StartRGB[1] = 1 - FadeColourMult( 0, 1, deltaT ) * G_TARGET;
		m_StartRGB[2] = 1 - FadeColourMult( 0, 1, deltaT ) * B_TARGET;
			
		SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_COLORIZATIONCOLOR,{m_StartRGB[0], m_StartRGB[1], m_StartRGB[2], 0.0},PPEGlow.L_23_TOXIC_TINT,PPOperators.MULTIPLICATIVE);
		SetTargetValueFloat(PPEExceptions.EXPOSURE,PPEExposureNative.PARAM_INTENSITY,false,m_ExpValue,PPEExposureNative.L_0_DEATH,PPOperators.SET);
		SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,m_BlurValue,PPEGaussFilter.L_0_SHOCK,PPOperators.ADD_RELATIVE);
	}

	protected void Run( float deltaT )
	{	
		SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_COLORIZATIONCOLOR,{m_StartRGB[0], m_StartRGB[1], m_StartRGB[2], 0.0},PPEGlow.L_23_TOXIC_TINT,PPOperators.MULTIPLICATIVE);
		SetTargetValueFloat(PPEExceptions.EXPOSURE,PPEExposureNative.PARAM_INTENSITY,false,m_ExpValue,PPEExposureNative.L_0_DEATH,PPOperators.SET);
		SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,m_BlurValue,PPEGaussFilter.L_0_SHOCK,PPOperators.ADD_RELATIVE);
	}

	protected void FadeOut( float deltaT )
	{
		m_ExpValue = EXP_TARGET - FadeColourMult( 0, EXP_TARGET, deltaT );
		m_BlurValue = BLUR_TARGET - FadeColourMult( 0, BLUR_TARGET, deltaT );

		m_StartRGB[0] = ( 1 - R_TARGET ) + FadeColourMult( 0, R_TARGET, deltaT );
		m_StartRGB[1] = ( 1 - G_TARGET ) + FadeColourMult( 0, G_TARGET, deltaT );
		m_StartRGB[2] = ( 1 - B_TARGET ) + FadeColourMult( 0, B_TARGET, deltaT );
		
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