#include "inclib.h"
#include "TextureLoader.h"
#include "Game.h"

TextureLoader::TextureLoader(Game* inGame)
{
	game = inGame;
	// Initialize COM
	auto res = CoInitialize(NULL); ZCHECK(res);

	// Create the COM imaging factory
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory2, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory)); //ZCHECK(hr);
}

TextureLoader::~TextureLoader()
{
	factory->Release();
}

HRESULT TextureLoader::LoadTextureFromFile(LPCWSTR fileName, ID3D11Texture2D*& texture, ID3D11ShaderResourceView*& textureView, bool generateMips, bool useSrgb, UINT frameIndex)
{
	IWICBitmapDecoder* decoder;

	factory->CreateDecoderFromFilename(fileName, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);

	IWICBitmapFrameDecode* frame;
	auto hr = decoder->GetFrame(frameIndex, &frame);

	IWICFormatConverter* converter;
	factory->CreateFormatConverter(&converter);

	converter->Initialize(frame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom);

	UINT width, height;
	converter->GetSize(&width, &height);

	UINT stride = width * 4;
	UINT bufSize = stride * height;

	BYTE* buf = new BYTE[bufSize];

	converter->CopyPixels(nullptr, stride, bufSize, buf);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.Format = useSrgb ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.MipLevels = generateMips ? 0 : 1;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buf;
	data.SysMemPitch = stride;
	data.SysMemSlicePitch = bufSize;

	game->Device->CreateTexture2D(&desc, generateMips ? nullptr : &data, &texture);
	if (texture != nullptr)
	{
		game->Device->CreateShaderResourceView(texture, nullptr, &textureView);

		if (generateMips) {
			game->Context->UpdateSubresource(texture, 0, nullptr, buf, stride, bufSize);
			if (textureView != 0)
			{
				game->Context->GenerateMips(textureView);
			}
		}
	}

	decoder->Release();
	frame->Release();
	converter->Release();
	delete[] buf;

	return S_OK;
}


