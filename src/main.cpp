#include "app.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void main_loop(void* arg) {
    AppContext* ctx = static_cast<AppContext*>(arg);
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            ctx->running = false;
        }
    }

    Uint64 currentTime = SDL_GetTicks();
    double frameTime = (currentTime - ctx->lastTime) / 1000.0;
    ctx->lastTime = currentTime;

    if (frameTime > 0.25) frameTime = 0.25;
    
    const double dt = 0.02;
    ctx->accumulator += frameTime;

    while (ctx->accumulator >= dt) {
        // On met à jour la physique de TOUS les pendules
        for (auto& p : ctx->pendulums) {
            p.update(dt * 6.5);
            
            float x1, y1, x2, y2;
            p.getPositions(ctx->cx, ctx->cy, x1, y1, x2, y2);
            p.trail.push_back({x2, y2});
            
            if (p.trail.size() > MAX_TRAIL_POINTS) {
                p.trail.erase(p.trail.begin());
            }
        }
        ctx->accumulator -= dt;
    }

    // Rendu - Fond noir profond
    SDL_SetRenderDrawColor(ctx->renderer, 5, 5, 10, 255);
    SDL_RenderClear(ctx->renderer);

    // 1. DESSIN DES TRAÎNÉES (On les dessine en premier pour que les tiges passent par-dessus)
    for (const auto& p : ctx->pendulums) {
        if (p.trail.size() > 1) {
            SDL_SetRenderDrawColor(ctx->renderer, p.color.r, p.color.g, p.color.b, p.color.a);
            for (size_t i = 0; i < p.trail.size() - 1; ++i) {
                SDL_RenderLine(ctx->renderer, p.trail[i].x, p.trail[i].y, p.trail[i + 1].x, p.trail[i + 1].y);
            }
        }
    }

    // 2. DESSIN DES TIGES ET DES JOINTS (Pour chaque pendule)
    for (auto& p : ctx->pendulums) {
        float x1, y1, x2, y2;
        p.getPositions(ctx->cx, ctx->cy, x1, y1, x2, y2);

        // Tiges en blanc semi-transparent pour ne pas masquer les tracés
        SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 120);
        SDL_RenderLine(ctx->renderer, ctx->cx, ctx->cy, x1, y1);
        SDL_RenderLine(ctx->renderer, x1, y1, x2, y2);
    }

    SDL_RenderPresent(ctx->renderer);
}

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return -1;
    }

    AppContext ctx;
    ctx.window = SDL_CreateWindow("Double Pendulum Chaos Theory", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!ctx.window) return -1;
    
    ctx.renderer = SDL_CreateRenderer(ctx.window, nullptr);
    if (!ctx.renderer) return -1;
    
    // Pour gérer la transparence des tiges
    SDL_SetRenderDrawBlendMode(ctx.renderer, SDL_BLENDMODE_BLEND);

    // INITIALISATION DES 3 PENDULES ALIGNÉS
    // On leur donne le même angle de base (2.0), mais avec un écart infime de 0.0001 rad
    double baseAngle = 2.0;
    
    // Pendule 1 : Cyan électrique
    ctx.pendulums.push_back(Pendulum(baseAngle, baseAngle, {0, 230, 255, 255}));
    
    // Pendule 2 : Rose Magenta (décalé de 0.0001 radian sur le second bras)
    ctx.pendulums.push_back(Pendulum(baseAngle, baseAngle + 0.0001, {255, 0, 150, 255}));
    
    // Pendule 3 : Vert Néon (décalé de 0.0002 radian)
    ctx.pendulums.push_back(Pendulum(baseAngle, baseAngle + 0.0002, {50, 255, 50, 255}));

    ctx.lastTime = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, &ctx, 0, 1);
#else
    while (ctx.running) {
        main_loop(&ctx);
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    SDL_Quit();
#endif

    return 0;
}