// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <vector>
#include <iostream>

#include "Ball.h"
#include "Racket.h"

using namespace std;


// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

//define nRow and nCol
#define nRow 6
#define nCol 10
#define total nRow*nCol
#define totalVertex (3*total*2)+12

// global declarations
IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
ID3D11Device* dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView* backbuffer;    // the pointer to our back buffer
ID3D11InputLayout* pLayout;            // the pointer to the input layout
ID3D11VertexShader* pVS;               // the pointer to the vertex shader
ID3D11PixelShader* pPS;                // the pointer to the pixel shader
ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer

// a struct to define a single vertex
//struct VERTEX { FLOAT X, Y, Z; D3DXCOLOR Color; };

//global variables
VERTEX OurVertices[totalVertex];
vector<bool> isBlockActive(total, true);

//Ball
Ball* ball;

//Racket
Racket* racket;

//alive
bool alive = true;

// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void RenderFrame(void);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders
void UpdateGraphics(void);  // updates the shape to render

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
    
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
        L"WindowClass",
        L"Our First Direct3D Program",
        WS_OVERLAPPEDWINDOW,
        300,
        300,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    InitD3D(hWnd);

    // enter the main loop:

    MSG msg;

    

    while (alive)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        RenderFrame();
    }

    // clean up DirectX and COM
    CleanD3D();

    //delete racket and ball
    delete racket;
    delete ball;


    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    } break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 4;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &dev,
        NULL,
        &devcon);


    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
}


// this is the function used to render a single frame
void RenderFrame(void)
{
    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    // select which vertex buffer to display
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

    // select which primtive type we are using
    devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    int currentVertex = 0;

    alive = !ball->Update();
    racket ->Update();

    ball->CheckCollision(racket->GetTopLeftVertex(), racket->GetTopRightVertex(), racket->GetBottomLeftVertex(), racket->GetBottomRightVertex());

    //update the graphics
    UpdateGraphics();



    //draw the ball
    devcon->Draw(6, currentVertex);
    currentVertex += 6;

    //draw the racket
    devcon->Draw(6, currentVertex);
    currentVertex += 6;
    int count = 0;
    int victoryCount = 0;

    for (bool b : isBlockActive) {
        if (b) {
            if (ball->CheckCollision(OurVertices[currentVertex], OurVertices[currentVertex + 1], OurVertices[currentVertex + 2], OurVertices[currentVertex + 4])) {
                isBlockActive[count] = false;
            }
            else {
                devcon->Draw(6, currentVertex);
            }
            
        }
        else {
			victoryCount++;
		}

        currentVertex += 6;
        count++;
    }

    if (victoryCount == total) {
		alive = false;
	}


    // draw the vertex buffer to the back buffer
    //devcon->Draw(totalVertex, 0);
    //devcon->Draw(3, 3);

    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}


// this is the function that creates the shape to render
void InitGraphics()
{

    //int tot = nRow * nCol;

    float spacerX = 2.f / (float)nCol;
    float spacerY = spacerX / 2.f;


    //vector<VERTEX> vertices = {};

    float y = 1.0f;
    float x = -1.0f;
    
    int index = 0;


    //add the ball
    OurVertices[index] = { 0.00f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;
    OurVertices[index] = { 0.075f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;
    OurVertices[index] = { 0.0f, -0.1f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;

    OurVertices[index] = { 0.075f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;
    OurVertices[index] = { 0.075f, -0.1f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;
    OurVertices[index] = { 0.0f, -0.1f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;

    ball = new Ball(OurVertices[0], OurVertices[1], OurVertices[2], OurVertices[4]);
    ball ->SetXSpeed(0.0001f);
    ball ->SetYSpeed(0.0001f);

    //add the racket
    OurVertices[index] = { -0.2f, -0.9f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;
    OurVertices[index] = { 0.2f, -0.9f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;
    OurVertices[index] = { -0.2f, -1.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;

    OurVertices[index] = { 0.2f, -0.9f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;
    OurVertices[index] = { 0.2f, -1.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;
    OurVertices[index] = { -0.2f, -1.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) };
    index++;

    racket = new Racket(OurVertices[index - 6], OurVertices[index - 5], OurVertices[index - 4], OurVertices[index - 2]);
    racket->SetXSpeed(0.00015f);


    for (int i = 0; i < nRow; i++) {
        for (int j = 0; j < nCol; j++) {

            //R, G, B values from 0 to 1
            float R = (float)rand() / (float)RAND_MAX;
            float G = (float)rand() / (float)RAND_MAX;
            float B = (float)rand() / (float)RAND_MAX;


            //first triangle
            OurVertices[index] = { x, y, 0.0f, D3DXCOLOR(R, G, B, 0.0f) };
            index++;
            OurVertices[index] = { x + spacerX, y, 0.0f, D3DXCOLOR(R, G, B, 0.0f) };
            index++;
            OurVertices[index] = { x, y - spacerY, 0.0f, D3DXCOLOR(R, G, B, 0.0f) };
            index++;

            //second triangle
            OurVertices[index] = { x + spacerX, y, 0.0f, D3DXCOLOR(R, G, B, 0.0f) };
            index++;
            OurVertices[index] = { x + spacerX, y - spacerY, 0.0f, D3DXCOLOR(R, G, B, 0.0f) };
            index++;
            OurVertices[index] = { x, y - spacerY, 0.0f, D3DXCOLOR(R, G, B, 0.0f) };
            index++;

            x = x + spacerX;

        }
        y -= spacerY;
        x = -1;
    }

    

    //VERTEX v = { 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) };
    //float test = v.Color.a;
    // create a triangle using the VERTEX struct
    /*VERTEX OurVertices[] =
    {
        {-1.0f, 1.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {0.0f, 1.0f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
        {-1.0f, 0.0f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)},


        {-1.0f, 0.0f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)},
        {0.0f, 1.0f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
        {0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},   
    };*/

    //creaqte the other half of the rectangle
    VERTEX OurVertices2[] =
    {
        {0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {-1.0f, 0.0f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)},
        {0.0f, 1.0f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)}
		
	};


    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * (totalVertex + 12);             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;

    // map the buffer
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    

    // copy the data
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 
    
    // unmap the buffer
    devcon->Unmap(pVBuffer, NULL);       
}

void UpdateGraphics()
{
    //update the first 6 vertices of the buffer
    OurVertices[0] = ball->GetTopLeftVertex();
    OurVertices[1] = ball->GetTopRightVertex();
    OurVertices[2] = ball->GetBottomLeftVertex();

    OurVertices[3] = ball->GetTopRightVertex();
    OurVertices[4] = ball->GetBottomRightVertex();
    OurVertices[5] = ball->GetBottomLeftVertex();

    //update the vertex of the racket
    OurVertices[6] = racket->GetTopLeftVertex();
    OurVertices[7] = racket->GetTopRightVertex();
    OurVertices[8] = racket->GetBottomLeftVertex();

    OurVertices[9] = racket->GetTopRightVertex();
    OurVertices[10] = racket->GetBottomRightVertex();
    OurVertices[11] = racket->GetBottomLeftVertex();
    


    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * (totalVertex + 6);             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    //dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    // map the buffer
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    // copy the data
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));
    // unmap the buffer
    devcon->Unmap(pVBuffer, NULL);

    cout << "UpdateGraphics" << endl;
}



// this function loads and prepares the shaders
void InitPipeline()
{
    // load and compile the two shaders
    ID3D10Blob* VS, * PS;
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

    // encapsulate both shaders into shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);
}
