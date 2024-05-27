// TreeGraphWidget.cpp

#include "TreeGraphWindow.h"
#include "Widgets/Views/STreeView.h"
#include "Widgets/Text/STextBlock.h"

void STreeGraphWindow::Construct(const FArguments& InArgs)
{
    SWindow::Construct(SWindow::FArguments()
        .Title(FText::FromString(TEXT("Tree Graph")))
        .ClientSize(FVector2D(800, 600))
        [
            SNew(STreeGraphWidget)
        ]);
}