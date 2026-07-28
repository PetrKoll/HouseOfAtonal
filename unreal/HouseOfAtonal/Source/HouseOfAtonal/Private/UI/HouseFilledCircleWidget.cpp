#include "UI/HouseFilledCircleWidget.h"

#include "Rendering/DrawElements.h"
#include "Styling/CoreStyle.h"
#include "Widgets/SLeafWidget.h"

class SHouseFilledCircle : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(SHouseFilledCircle)
		: _Color(FLinearColor::White)
	{}
		SLATE_ARGUMENT(FLinearColor, Color)
	SLATE_END_ARGS()

	void Construct(const FArguments& Arguments)
	{
		Color = Arguments._Color;
		SetCanTick(false);
	}

	void SetColor(const FLinearColor& NewColor)
	{
		Color = NewColor;
		Invalidate(EInvalidateWidgetReason::Paint);
	}

	virtual FVector2D ComputeDesiredSize(float) const override
	{
		return FVector2D(100.0f);
	}

	virtual int32 OnPaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override
	{
		constexpr int32 SegmentCount = 48;
		const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
		const FVector2D LocalCenter = LocalSize * 0.5f;
		const float Radius = FMath::Min(LocalSize.X, LocalSize.Y) * 0.5f;
		const FColor VertexColor =
			(Color * InWidgetStyle.GetColorAndOpacityTint()).ToFColor(true);

		TArray<FSlateVertex> Vertices;
		Vertices.SetNumZeroed(SegmentCount + 1);
		Vertices[0].Position =
			FVector2f(AllottedGeometry.LocalToAbsolute(LocalCenter));
		Vertices[0].TexCoords[2] = 1.0f;
		Vertices[0].TexCoords[3] = 1.0f;
		Vertices[0].Color = VertexColor;

		for (int32 Index = 0; Index < SegmentCount; ++Index)
		{
			const float Angle =
				2.0f * PI * static_cast<float>(Index) /
				static_cast<float>(SegmentCount);
			const FVector2D LocalPoint =
				LocalCenter +
				FVector2D(FMath::Cos(Angle), FMath::Sin(Angle)) * Radius;
			FSlateVertex& Vertex = Vertices[Index + 1];
			Vertex.Position =
				FVector2f(AllottedGeometry.LocalToAbsolute(LocalPoint));
			Vertex.TexCoords[2] = 1.0f;
			Vertex.TexCoords[3] = 1.0f;
			Vertex.Color = VertexColor;
		}

		TArray<SlateIndex> Indices;
		Indices.Reserve(SegmentCount * 3);
		for (int32 Index = 0; Index < SegmentCount; ++Index)
		{
			Indices.Add(0);
			Indices.Add(static_cast<SlateIndex>(Index + 1));
			Indices.Add(static_cast<SlateIndex>(
				Index + 1 == SegmentCount ? 1 : Index + 2));
		}

		const FSlateBrush* WhiteBrush =
			FCoreStyle::Get().GetBrush(TEXT("WhiteBrush"));
		FSlateDrawElement::MakeCustomVerts(
			OutDrawElements,
			LayerId,
			WhiteBrush->GetRenderingResource(),
			Vertices,
			Indices,
			nullptr,
			0,
			0);
		return LayerId;
	}

private:
	FLinearColor Color;
};

void UHouseFilledCircleWidget::SetFillColor(FLinearColor NewColor)
{
	FillColor = NewColor;
	if (CircleWidget.IsValid())
	{
		CircleWidget->SetColor(FillColor);
	}
}

TSharedRef<SWidget> UHouseFilledCircleWidget::RebuildWidget()
{
	CircleWidget = SNew(SHouseFilledCircle).Color(FillColor);
	return CircleWidget.ToSharedRef();
}

void UHouseFilledCircleWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (CircleWidget.IsValid())
	{
		CircleWidget->SetColor(FillColor);
	}
}

void UHouseFilledCircleWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	CircleWidget.Reset();
}
