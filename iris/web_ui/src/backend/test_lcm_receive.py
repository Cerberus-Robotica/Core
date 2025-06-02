import lcm

def handler_debug(channel, data):
    print(f"[RECEBIDO] Canal: {channel} | Tamanho: {len(data)} bytes")

lc = lcm.LCM()
lc.subscribe("GC", handler_debug)
lc.subscribe("vision", handler_debug)
lc.subscribe("tartarus", handler_debug)
lc.subscribe("IA", handler_debug)

print("Aguardando mensagens LCM nos canais: GC, vision, tartarus, IA...")
while True:
    lc.handle()
