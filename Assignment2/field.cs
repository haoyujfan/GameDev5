using Godot;
using System;

public partial class field : Node3D
{
	//var player = new Player();
	//private Player player;
	private RichTextLabel Lives;
	private RichTextLabel Effects;
	private RichTextLabel Rotate;
	private RichTextLabel Music;
	private int PointsValue;
	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		//player = GetNode<Player>("Player");
		Lives = GetNode<RichTextLabel>("Lives");
		Effects = GetNode<RichTextLabel>("SoundEffectToggle");
		Rotate = GetNode<RichTextLabel>("RotateToggle");
		Music = GetNode<RichTextLabel>("MusicToggle");
		PointsValue = 0;
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
		//Points.Text = "Points: " + player.Call(Player.get_points());
		//Points.Text = "Points: " + GetNode<Player>("Player").get_points();
		Lives.Text = "Lives: " + PointsValue.ToString();
	}
	
	private void _on_player_interact_orange()
	{
		PointsValue++;
	}
	
	private void _on_cactus_interact_cactus()
	{
		PointsValue--;
	}
	private void _on_player_sound_effect_toggle(string toggle)
	{
		Effects.Text = "Sound Effect Mute: , " + toggle;
	}
	
	private void _on_player_rotate_mode_toggle(string toggle)
	{
		Rotate.Text = "Rotate Mode: R " + toggle;
	}
	
	private void _on_ground_music_toggle(string toggle)
	{
		Music.Text = "Music Mute: M " + toggle;
	}


}








