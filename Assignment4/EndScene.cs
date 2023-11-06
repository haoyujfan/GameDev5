using Godot;
using System;

public partial class EndScene : Node2D
{
	// restart button
	private void _on_restart_pressed()
	{
		GetTree().ChangeSceneToFile("res://scenes/start_menu.tscn");
	}
	
	// quit button
	private void _on_quit_pressed()
	{
		GetTree().Quit();
	}
}
