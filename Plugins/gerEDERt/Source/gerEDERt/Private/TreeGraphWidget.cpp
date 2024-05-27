// TreeGraphWidget.cpp

#include "TreeGraphWidget.h"
#include "Widgets/Views/STreeView.h"
#include "Widgets/Text/STextBlock.h"

void STreeGraphWidget::Construct(const FArguments& InArgs)
{
    PopulateTree();

    float InitialYOffset = 20.0f;

    TSharedPtr<SOverlay> TreeOverlay = SNew(SOverlay);

    ConstructTree(RootNode, 400.0f, InitialYOffset, TreeOverlay);

    ChildSlot
        [
            SNew(SBox)
                .WidthOverride(800)
                .HeightOverride(600)
                [
                    TreeOverlay.ToSharedRef()
                ]
        ];
}

void STreeGraphWidget::PopulateTree()
{
    // Sample data, replace with your actual data
    RootNode = MakeShareable(new FTreeNode(TEXT("Root")));
    TSharedPtr<FTreeNode> Child1 = MakeShareable(new FTreeNode(TEXT("Child1")));
    TSharedPtr<FTreeNode> Child2 = MakeShareable(new FTreeNode(TEXT("Child2")));
    TSharedPtr<FTreeNode> GrandChild1 = MakeShareable(new FTreeNode(TEXT("GrandChild1")));

    RootNode->Children.Add(Child1);
    RootNode->Children.Add(Child2);
    Child1->Children.Add(GrandChild1);
}

void STreeGraphWidget::ConstructTree(TSharedPtr<FTreeNode> Node, float XOffset, float& YOffset, TSharedPtr<SOverlay> ParentOverlay)
{
    if (!Node.IsValid())
    {
        return;
    }

    TSharedPtr<STextBlock> NodeTextBlock = SNew(STextBlock).Text(FText::FromString(Node->Name));

    // Set the position and size of the node box
    TSharedPtr<SBox> NodeBox = SNew(SBox)
        .WidthOverride(100)
        .HeightOverride(50)
        [
            NodeTextBlock.ToSharedRef()
        ];

    // Add the node box to the overlay
    ParentOverlay->AddSlot()
        [
            SNew(SCanvas)
                + SCanvas::Slot()
                .Position(FVector2D(XOffset, YOffset))
                .Size(FVector2D(100, 50))
                [
                    NodeBox.ToSharedRef()
                ]
        ];

    Nodes.Add(NodeTextBlock);

    float ChildYOffset = YOffset + 100.0f;
    float ChildXOffset = XOffset + 150.0f;

    for (TSharedPtr<FTreeNode> ChildNode : Node->Children)
    {
        // Draw a line connecting the parent and child nodes
        ParentOverlay->AddSlot()
            [
                SNew(SCanvas)
                    + SCanvas::Slot()
                    .Position(FVector2D(XOffset + 100, YOffset + 25))
                    .Size(FVector2D(50, 2))
                    [
                        SNew(SBox)
                            .WidthOverride(50)
                            .HeightOverride(2)
                            [
                                SNew(SImage)
                                    .ColorAndOpacity(FLinearColor::Black)
                            ]
                    ]
            ];

        ParentOverlay->AddSlot()
            [
                SNew(SCanvas)
                    + SCanvas::Slot()
                    .Position(FVector2D(XOffset + 150, YOffset + 25))
                    .Size(FVector2D(2, 100))
                    [
                        SNew(SBox)
                            .WidthOverride(2)
                            .HeightOverride(100)
                            [
                                SNew(SImage)
                                    .ColorAndOpacity(FLinearColor::Black)
                            ]
                    ]
            ];

        ConstructTree(ChildNode, ChildXOffset, ChildYOffset, ParentOverlay);
        ChildYOffset += 100.0f;
    }
}