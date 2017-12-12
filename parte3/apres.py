#!/usr/bin/python3
import pygame as pg
import fileinput

from math import *

pg.init()                                 # inicialização do sistema

# altura e largura da arena, em células
H, W = 20, 20

TAMx, TAMy = 800, 600

# largura da célula
l = TAMx/W*4/3
# altura da célula
L = TAMy/H


# cria a janela
scr = pg.display.set_mode((TAMx+ceil(l/4),TAMy+ceil(L/2)))
# só para simplificar, atualização da imagem
atu = pg.display.update


# pinta o fundo da janela
scr.fill((0,200,0))
atu()

def convert(i ,j):
    """
    Converte a coordenada (i,j) para a posição em pixels na janela
    """
    x = 0.5*l + 0.75*i*l
    y = 0.5*L + 0.5*j*L

    return x,y

class Robô(pg.sprite.Sprite):
    """
    Representa um Robô, possui uma imagem
    """
    def __init__(s, img):
        """
        Construtor, img é o nome do arquivo com a imagem
        """
        pg.sprite.Sprite.__init__(s)
        s.image = pg.image.load(img)
        s.image = pg.transform.scale(s.image, (26, 26))
        s.rect = s.image.get_rect()

    def draw(s, i,j):
        """
        Desenha o robô na célula (i,j)
        """
        x, y = convert(i,j)

        # fronteiras do sprite
        l0,h0,l1,h1 = s.rect

        # corrije a posição de acordo com o tamanho do sprite
        s.rect.topleft = [x-l1/2,y-h1/2]
        # desenha
        scr.blit(s.image, s.rect)



class Base(pg.sprite.Sprite):
    """
    Representa um Robô, possui uma imagem
    """
    def __init__(s, img):
        """
        Construtor, img é o nome do arquivo com a imagem
        """
        pg.sprite.Sprite.__init__(s)
        s.image = pg.image.load(img)
        s.image = pg.transform.scale(s.image, (26, 26))
        s.rect = s.image.get_rect()

    def draw(s, i,j):
        """
        Desenha o robô na célula (i,j)
        """
        x, y = convert(i,j)

        # fronteiras do sprite
        l0,h0,l1,h1 = s.rect

        # corrije a posição de acordo com o tamanho do sprite
        s.rect.topleft = [x-l1/2,y-h1/2]
        # desenha
        scr.blit(s.image, s.rect)



class Cristal:
    """
    Representa um Cristal, possui uma imagem
    """
    def __init__(s):
        """
        Construtor, img é o nome do arquivo com a imagem
        """
        s.quanti = 0
        pg.sprite.Sprite.__init__(s)
        s.image = pg.image.load('footage/cristal.png')
        s.rect = s.image.get_rect()


    def draw(s, i,j):
        """
        Desenha o Cristal na célula (i,j)
        """
        x, y = convert(i,j)

        # fronteiras do sprite
        l0,h0,l1,h1 = s.rect

        # corrije a posição de acordo com o tamanho do sprite
        s.rect.topleft = [x-l1/2,y-h1/2]
        # desenha
        scr.blit(s.image, s.rect)

    
class cell:
    """
    Representa uma das células da arena
    """
    def __init__(s,i,j):
        """
        Construtor, i e j formam a coordenada
        """
        # Posição do centro
        s.cx, s.cy = convert(i,j)
        # Cor (pode ser passada para o construtor no futuro)
        s.cor = (200,200,200)
        s.crist = 0

        # Vértices do hexágono
        s.pontos = (
            (s.cx-l*0.5 , s.cy),
            (s.cx-l*0.25, s.cy-L/2),
            (s.cx+l*0.25, s.cy-L/2),
            (s.cx+l*0.5 , s.cy),
            (s.cx+l*0.25, s.cy+L/2),
            (s.cx-l*0.25, s.cy+L/2),
            )
        
    def draw(s) :
        """
        Desenha a célula
        """
        pg.draw.polygon(scr, s.cor, s.pontos, 0)
        pg.draw.lines(scr, (0,0,0), True, s.pontos, 1)


# Construção da matriz que representa a arena
arena = []
for i in range(H):
    arena.append([])
    for j in range(W*2):
        arena[i].append(cell(i,j))
        if not((i % 2 == 0 and j % 2 != 0) or (j % 2 == 0 and i % 2 != 0)):
            arena[i][j].draw()

# lista dos robôs definidos
robs = []

# lista de bases
bases = []

# atualiza a janela
atu()

cont = 0

# Leitura dos comandos pela entrada padrão ou de um arquivo
for line in fileinput.input():
    r = line.split()

    # linha começando com 'fim', termina o programa
    if r[0] == 'fim':
        break

    if r[0] == 'base':
        bases.append(Base(r[1]))
        r = r[2:]
        oi, oj = list(map(int, r))

        # desenha o robô na célula de destino
        if 0 <= oi  < W and 0 <= oj < H:
            bases[cont].draw(oi,oj)
            cont = cont + 1

        # tempo de espera para atualização, apenas para testes
        pg.time.wait(1000) 
        atu()
        continue

    if r[0] == 'recolhe':
        r = r[1:]
        ni, oi, oj = list(map(int, r))

        if arena[oi][oj].quanti - ni <= 0:
            arena[oi][oj] = cell(oi, oj)
            arena[oi][oj].draw()

        # tempo de espera para atualização, apenas para testes
        pg.time.wait(1000) 
        atu()
        continue

    if r[0] == 'cristais':
        r = r[1:]
        ni, oi, oj = list(map(int, r))
        arena[oi][oj] = Cristal()
        arena[oi][oj].quanti = ni

        # desenha o cristal na célula de destino
        if 0 <= oi  < W and 0 <= oj < H:
            arena[oi][oj].draw(oi,oj)

        # tempo de espera para atualização, apenas para testes
        pg.time.wait(1000) 
        atu()
        continue

    if r[0] == 'assassinato':
        r = r[1:]
        ri, oi, oj = list(map(int, r))

        # remove o robô da lista quando morrer
        robs.remove(robs[ri])

        # redesenha a célula original (apaga o robô)
        if 0 <= oi  < W and 0 <= oj < H:
            arena[oi][oj].draw()


        # tempo de espera para atualização, apenas para testes
        pg.time.wait(1000) 
        atu()
        continue

    # linha começando com 'rob', inclui um robô,
    # o segundo argumento na linha é a imagem
    if r[0] == 'rob':
        robs.append(Robô(r[1]))
        continue

    if r[0] == 'move':
        # linha de movimentação:
        # robô, corrdenadas originais, coordenadas finais
        r = r[1:]
        ri, oi, oj, di, dj = list(map(int, r))


        # redesenha a célula original (apaga o robô)
        arena[oi][oj].draw()
        # desenha o robô na célula de destino
        robs[ri].draw(di,dj)

        # tempo de espera para atualização, apenas para testes
        pg.time.wait(1000) 
        atu()
        continue

pg.quit()
# Local variables:
# mode: python
# End:
