using Godot;
using System;

public partial class StartMenu : Node2D
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
		GD.Print("Player Connected: " + id.ToString());
	}

	// runs when a peer disconnects (runs on all peers)
	// id - id of player that disconnected
	private void PeerDisconnected(long id) {
		GD.Print("Player Disconnected: " + id.ToString());
	}

	// runs when the connection is successful (only runs on clients)
	private void ConnectedToServer() {
		GD.Print("Connected Successfully!");
		RpcId(1, "SendPlayerInformation", GetNode<LineEdit>("Username input").Text, Multiplayer.GetUniqueId());
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
			GD.Print("Error! Cannot Host!: " + error.ToString());
			return;
		}
		// setup compression type and self as peer
		peer.Host.Compress(ENetConnection.CompressionMode.RangeCoder);
		Multiplayer.MultiplayerPeer = peer;
		GD.Print("Host Joined! Waiting For Other Players :: ");
		SendPlayerInformation(GetNode<LineEdit>("Username input").Text, 1);
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
		Rpc("StartGame");
	}

	[Rpc(MultiplayerApi.RpcMode.AnyPeer, CallLocal = true, TransferMode = MultiplayerPeer.TransferModeEnum.Reliable)]
	private void StartGame() {
		foreach (var item in GameManager.Players) {
			GD.Print(item.Name + " is playing");
		}
		GetTree().ChangeSceneToFile("res://scenes/main.tscn");
	}
	
	private void _on_exit_pressed()
	{
		GetTree().Quit();
	}
	
	[Rpc(MultiplayerApi.RpcMode.AnyPeer)]
	private void SendPlayerInformation(string name, int id) {
		PlayerInfo playerInfo = new PlayerInfo(){
			Name = name,
			Id = id
		};
		if (!GameManager.Players.Contains(playerInfo)) {
			GameManager.Players.Add(playerInfo);
		}
		if (Multiplayer.IsServer()) {
			foreach (var item in GameManager.Players) {
				Rpc("SendPlayerInformation", name, id);
			}
		}
	}
}


