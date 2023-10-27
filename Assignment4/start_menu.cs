using Godot;
using System;

public partial class start_menu : Node2D
{
	[Export]
	private int port = 8000;

	[Export]
	private string address = "127.0.0.1"; // local host adapter on local computer
	
	private ENetMultiplayerPeer peer;
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
		peer = new ENetMultiplayerPeer();
		// create server and check for issues
		var error = peer.CreateServer(port, 2); // 2 max clients
		if (error != Error.Ok) {
			GD.Print("Error !! Cannot Host! :: " + error.ToString());
			return;
		}
		// setup compression type and self as peer
		peer.Host.Compress(ENetConnection.CompressionMode.RangeCoder);
		Multiplayer.MultiplayerPeer = peer;
		GD.Print("Set Up Server! Waiting For Players To Connect :: ");
	}

	private void _on_join_button_down()
	{
		peer = new ENetMultiplayerPeer();
		peer.CreateClient(address, port);

		// must use same compression type as host
		peer.Host.Compress(ENetConnection.CompressionMode.RangeCoder);
		Multiplayer.MultiplayerPeer = peer;
		GD.Print("Joined Game!");
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


