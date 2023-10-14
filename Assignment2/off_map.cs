using Godot;
using System;

public partial class off_map : Node2D
{
	// restart button
	private void _on_restart_pressed()
	{
		GetTree().ChangeSceneToFile("res://scenes/main.tscn");
	}
	
	// quit button
	private void _on_quit_pressed()
	{
		GetTree().Quit();
	}

}
