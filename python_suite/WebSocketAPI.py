#!/usr/bin/env python3
"""
WebSocket API Server - Real-time Metrics & Remote Control
Features: Live Metrics, Remote Diagnostics, Multi-client Support
Python 3.8+ | PyInstaller compatible
"""

import asyncio
import json
import logging
from datetime import datetime
from typing import Dict, Set
from collections import deque

try:
    import aiohttp
    from aiohttp import web
    import websockets
except ImportError:
    print("[ERROR] aiohttp/websockets not installed")
    print("[INFO] Install: pip install aiohttp websockets")
    exit(1)

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class WebSocketServer:
    """Real-time WebSocket API server"""
    
    def __init__(self, host: str = 'localhost', port: int = 8765):
        self.host = host
        self.port = port
        self.clients: Set[web.WebSocketResponse] = set()
        self.metrics_history: deque = deque(maxlen=3600)
        self.server = None
    
    async def websocket_handler(self, request):
        """Handle WebSocket connections"""
        ws = web.WebSocketResponse()
        await ws.prepare(request)
        self.clients.add(ws)
        
        logger.info(f"[CONNECT] Client connected from {request.remote}")
        logger.info(f"[CLIENTS] Total clients: {len(self.clients)}")
        
        try:
            async for msg in ws.iter_any():
                if isinstance(msg, str):
                    try:
                        data = json.loads(msg)
                        response = await self.handle_command(data)
                        await ws.send_str(json.dumps(response))
                    except json.JSONDecodeError:
                        await ws.send_str(json.dumps({'error': 'Invalid JSON'}))
        
        except Exception as e:
            logger.error(f"[ERROR] WebSocket error: {e}")
        
        finally:
            self.clients.discard(ws)
            logger.info(f"[DISCONNECT] Client disconnected")
            logger.info(f"[CLIENTS] Total clients: {len(self.clients)}")
        
        return ws
    
    async def handle_command(self, data: Dict) -> Dict:
        """Handle incoming commands"""
        command = data.get('command')
        args = data.get('args', {})
        
        if command == 'get_metrics':
            return {'metrics': list(self.metrics_history)[-100:]}
        
        elif command == 'ping':
            target = args.get('target', '8.8.8.8')
            return {'command': 'ping', 'target': target, 'status': 'pending'}
        
        elif command == 'get_status':
            return {
                'status': 'running',
                'timestamp': datetime.now().isoformat(),
                'metrics_count': len(self.metrics_history),
                'connected_clients': len(self.clients)
            }
        
        else:
            return {'error': f'Unknown command: {command}'}
    
    async def broadcast_metrics(self, metrics: Dict):
        """Broadcast metrics to all connected clients"""
        self.metrics_history.append(metrics)
        
        message = json.dumps({
            'type': 'metrics',
            'data': metrics,
            'timestamp': datetime.now().isoformat()
        })
        
        for client in self.clients:
            try:
                await client.send_str(message)
            except Exception as e:
                logger.error(f"[ERROR] Failed to send to client: {e}")
    
    async def start_server(self):
        """Start WebSocket server"""
        app = web.Application()
        app.router.add_get('/ws', self.websocket_handler)
        
        runner = web.AppRunner(app)
        await runner.setup()
        site = web.TCPSite(runner, self.host, self.port)
        
        await site.start()
        logger.info(f"[SERVER] WebSocket server started at ws://{self.host}:{self.port}/ws")
        
        try:
            await asyncio.Event().wait()
        except KeyboardInterrupt:
            logger.info("[SERVER] Shutting down...")
            await runner.cleanup()

async def main():
    """Main function"""
    server = WebSocketServer(host='0.0.0.0', port=8765)
    
    # Demo: Send sample metrics every second
    async def send_demo_metrics():
        import random
        while True:
            await asyncio.sleep(1)
            metrics = {
                'latency': 25 + random.gauss(0, 5),
                'packet_loss': max(0, random.gauss(0.5, 0.5)),
                'bandwidth': 85 + random.gauss(0, 10)
            }
            await server.broadcast_metrics(metrics)
    
    # Run server and demo
    await asyncio.gather(
        server.start_server(),
        send_demo_metrics()
    )

if __name__ == '__main__':
    print("""
    ╔════════════════════════════════════════════════════════╗
    ║  WebSocket API Server v2.0                             ║
    ║  Real-time Metrics • Remote Control                    ║
    ╚════════════════════════════════════════════════════════╝
    """)
    
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nServer stopped.")
