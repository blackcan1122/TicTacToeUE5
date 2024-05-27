// TreeGraphWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Text/STextBlock.h"

class GEREDERT_API STreeGraphWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(STreeGraphWidget) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    struct FTreeNode
    {
        FString Name;
        TArray<TSharedPtr<FTreeNode>> Children;

        FTreeNode(const FString& InName) : Name(InName) {}
    };

    TSharedPtr<FTreeNode> RootNode;
    void PopulateTree();
    void ConstructTree(TSharedPtr<FTreeNode> Node, float XOffset, float& YOffset, TSharedPtr<SOverlay> ParentOverlay);

    TArray<TSharedPtr<STextBlock>> Nodes;
};