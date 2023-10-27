using Godot;
using System;

public partial class start_menu : Node2D
{
	public override void _Ready() {
		Multiplayer.PeerConnected += PeerConnected;
		Multiplayer.PeerDisconnected += PeerDisconnected;
		Multiplayer.ConnectedToServer += ConnectedToServer;
		Multiplayer.ConnectionFailed += ConnectionFailed;
	}

	// runs when a player connects (runs on all peers)
	// id - id of player that connected
	private void PeerConnected(long id) {
		GD.Print("Player Connected" + id.ToString());
	}

	// runs when a peer disconnects (runs on all peers)
	// id - id of player that disconnected
	private void PeerDisconnected(long id) {
		GD.Print("Player Disconnected" + id.ToString());
	}

	// runs when the connection is successful (only runs on clients)
	private void ConnectedToServer() {
		GD.Print("CONNECTED SUCCESSFULLY !!!");
	}

	// runs when connection fails (only runs on clients)
	private void ConnectionFailed() {
		GD.Print("CONNECTION FAILED !!!");
	}
	
	private void _on_host_button_down()
	{
		// Replace with function body.
	}

	private void _on_join_button_down()
	{
		// Replace with function body.
	}

	private void _on_start_pressed()
	{
		GetTree().ChangeSceneToFile("res://scenes/main.tscn");
	}
	
	private void _on_exit_pressed()
	{
		GetTree().Quit();
	}
}


